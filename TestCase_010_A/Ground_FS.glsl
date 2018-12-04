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
uniform vec3  u_DiffuseColor;
uniform vec3  u_SpecularColor;
uniform vec3  u_LightPosition;

uniform float u_Roughness;
uniform float u_Intensity;

uniform sampler2D u_LTC_MatrixTexture;
uniform sampler2D u_LTC_MagnitueTexture;

const float PI = 3.14159265;

vec3 integrateLTC(vec3 vNormal, vec3 vViewDir, vec3 vFragPos, mat3 vLTCMatrix)
{	
	//着色点上的切线空间正交基
	vec3 Tangent = normalize(vViewDir - vNormal * dot(vViewDir, vNormal));
	vec3 Bitangent = cross(vNormal, Tangent);

	//将变换矩阵转换到切线空间
	vLTCMatrix = vLTCMatrix * transpose(mat3(Tangent, Bitangent, vNormal));
	vec3 LightPositionInTangentSpace = vLTCMatrix * (u_LightPosition - vFragPos);
	if(LightPositionInTangentSpace.z < 0)
		return vec3(0);
	LightPositionInTangentSpace = normalize(LightPositionInTangentSpace);
	vec3 VSum = vec3(LightPositionInTangentSpace.z / PI);

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

void main()
{
	vec3 GroundNormal = vec3(0, 1, 0);	//其他几何体的话应该由其法线乘以模型矩阵来算
	vec3 ViewDir = normalize(u_CameraPosInWorldSpace - v2f_FragPosInWorldSpace);

	vec2 UV = LTC_Coords(dot(GroundNormal, ViewDir), u_Roughness);

	vec4 LTCMatrixComponents = texture2D(u_LTC_MatrixTexture, UV);
	mat3 LTCMatrix = mat3
	(
		vec3(1, 0, LTCMatrixComponents.y),
		vec3(0, LTCMatrixComponents.z, 0),
		vec3(LTCMatrixComponents.w, 0, LTCMatrixComponents.x)
	);

	//vec3 LightDir = normalize(u_LightPosition - v2f_FragPosInWorldSpace);

	vec3 Diffuse = integrateLTC(GroundNormal, ViewDir, v2f_FragPosInWorldSpace, mat3(1));
	vec3 Specular = integrateLTC(GroundNormal, ViewDir, v2f_FragPosInWorldSpace, LTCMatrix);
	vec2 Schlick = texture2D(u_LTC_MagnitueTexture, UV).xy;
	Specular *= u_SpecularColor * Schlick.x + (1.0 - u_SpecularColor) * Schlick.y;

	vec3 ResultColor = u_Intensity * (Diffuse * u_DiffuseColor + Specular);
	//vec3 ResultColor = u_Intensity * Diffuse * u_DiffuseColor;

	vec3 ReinhardMappedColor = ResultColor / (ResultColor + vec3(1.0));
	vec3 GammaedColor = pow(ReinhardMappedColor, vec3(1.0 / 2.2));

	FragColor_ = vec4(GammaedColor, 1.0);
}