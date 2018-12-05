#version 430 core

in  vec3 v2f_FragPosInWorldSpace;
in  vec2 v2f_TexCoords;
out vec4 FragColor_;

layout (std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform vec3  u_CameraPosInWorldSpace;
uniform vec3  u_Albedo = vec3(1);
uniform vec4  u_DielectricSpecularColor = vec4(0.220916301, 0.220916301, 0.220916301, 1.0 - 0.220916301);
uniform vec3  u_LightColor = vec3(1);
uniform vec3  u_LightPosition;

uniform float u_Roughness;
uniform float u_Intensity;
uniform float u_Metalness = 0.0;

uniform sampler2D u_LTC_MatrixTexture;
uniform sampler2D u_LTC_MagnitueTexture;

const float PI = 3.14159265;

vec3 integrateLTCSpecular(vec3 vNormal, vec3 vViewDir, vec3 vFragPos, mat3 vLTCMatrix)
{	
	//将变换矩阵转换到切线空间
	float JacobianNom = determinant(vLTCMatrix);
	vec3 LightPositionInTangentSpace = vLTCMatrix * (u_LightPosition - vFragPos);
	if(LightPositionInTangentSpace.z < 0)
		return vec3(0);
	float JacobianDenom = pow(length(LightPositionInTangentSpace), 3);
	LightPositionInTangentSpace = normalize(LightPositionInTangentSpace);
	vec3 VSum = vec3(LightPositionInTangentSpace.z/* / PI*/);
	VSum *= JacobianNom / JacobianDenom;

	return VSum;
}

vec3 integrateLTCDiffuse(vec3 vNormal, vec3 vViewDir, vec3 vFragPos, mat3 vLTCMatrix)
{
	//将变换矩阵转换到切线空间
	vec3 LightPositionInTangentSpace = vLTCMatrix * (u_LightPosition - vFragPos);
	if(LightPositionInTangentSpace.z < 0)
		return vec3(0);
	LightPositionInTangentSpace = normalize(LightPositionInTangentSpace);
	vec3 VSum = vec3(LightPositionInTangentSpace.z/* / PI*/);

	return VSum;
}

vec2 LTC_Coords(float vCosTheta, float vRoughness)
{
	float Theta = acos(vCosTheta);
    vec2 Coords = vec2(vRoughness, Theta/(0.5 * PI));
    const float LUT_SIZE = 32.0;
    // scale and bias coordinates, for correct filtered lookup
    Coords = Coords * (LUT_SIZE - 1.0)/LUT_SIZE + 0.5/LUT_SIZE;
    return Coords;
}

float OneMinusReflectivityFromMetallic(float metallic)
{
    // We'll need oneMinusReflectivity, so
    //   1-reflectivity = 1-lerp(dielectricSpec, 1, metallic) = lerp(1-dielectricSpec, 0, metallic)
    // store (1-dielectricSpec) in unity_ColorSpaceDielectricSpec.a, then
    //   1-reflectivity = lerp(alpha, 0, metallic) = alpha + metallic*(0 - alpha) =
    //                  = alpha - metallic * alpha
    float oneMinusDielectricSpec = u_DielectricSpecularColor.a;
    return oneMinusDielectricSpec - metallic * oneMinusDielectricSpec;
}

vec3 DiffuseAndSpecularFromMetallic (vec3 albedo, float metallic, out vec3 specColor)
{
    specColor = mix (u_DielectricSpecularColor.rgb, albedo, metallic);
    float oneMinusReflectivity = OneMinusReflectivityFromMetallic(metallic);
    return u_Albedo * oneMinusReflectivity;
}

vec3 FresnelSchlickRoughness(float vCosTheta, vec3 vF0, float vRoughness)
{
	return vF0 + (max(vec3(1.0 - vRoughness), vF0) - vF0) * pow(1.0 - vCosTheta, 5.0);
}

void main()
{
	vec3 GroundNormal = vec3(0, 1, 0);	//其他几何体的话应该由其法线乘以模型矩阵来算
	vec3 ViewDir = normalize(u_CameraPosInWorldSpace - v2f_FragPosInWorldSpace);
	vec3 LightDir = normalize(u_LightPosition - v2f_FragPosInWorldSpace);
	vec3 H = normalize(ViewDir + LightDir);
	float Distance = length(u_LightPosition - v2f_FragPosInWorldSpace);	//length可以变成dot，减少开方运算
	float LightAttenuation = 1.0f / (Distance * Distance);

	vec3 SpecularColor;
	vec3 DiffuseColor = DiffuseAndSpecularFromMetallic (u_Albedo, u_Metalness, /*out*/ SpecularColor);
	vec3 F0 = SpecularColor;
	vec3 F = FresnelSchlickRoughness(max(dot(H, ViewDir), 0.0f), F0, u_Roughness);
	vec3 Kd = vec3(1.0) - F;

	vec2 UV = LTC_Coords(dot(GroundNormal, ViewDir), u_Roughness);

	vec4 LTCMatrixComponents = texture2D(u_LTC_MatrixTexture, UV);
	mat3 LTCMatrix = mat3
	(
		vec3(1, 0, LTCMatrixComponents.y),
		vec3(0, LTCMatrixComponents.z, 0),
		vec3(LTCMatrixComponents.w, 0, LTCMatrixComponents.x)
	);

	//着色点上的切线空间正交基
	vec3 Tangent = normalize(ViewDir - GroundNormal * dot(ViewDir, GroundNormal));
	vec3 Bitangent = cross(GroundNormal, Tangent);
	mat3 TangentSpaceInverseMatrix = transpose(mat3(Tangent, Bitangent, GroundNormal));

	vec3 Diffuse = integrateLTCDiffuse(GroundNormal, ViewDir, v2f_FragPosInWorldSpace, TangentSpaceInverseMatrix);
	vec3 Specular = integrateLTCSpecular(GroundNormal, ViewDir, v2f_FragPosInWorldSpace, LTCMatrix * TangentSpaceInverseMatrix);
	vec2 Schlick = texture2D(u_LTC_MagnitueTexture, UV).xy;
	Specular *= SpecularColor * Schlick.x + (1.0 - SpecularColor) * Schlick.y;

	vec3 ResultColor = u_Intensity * (Diffuse * DiffuseColor * Kd + Specular) * LightAttenuation;
	//vec3 ResultColor = u_Intensity * Diffuse * DiffuseColor * LightAttenuation * Kd;
	//vec3 ResultColor = u_Intensity * Specular * LightAttenuation;

	//vec3 ReinhardMappedColor = ResultColor / (ResultColor + vec3(1.0));
	//vec3 GammaedColor = pow(ReinhardMappedColor, vec3(1.0 / 2.2));
	vec3 GammaedColor = ResultColor;

	FragColor_ = vec4(GammaedColor, 1.0);
}