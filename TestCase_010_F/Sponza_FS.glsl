#version 430 core

in  vec3 v2f_FragPosInWorldSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;
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

uniform bool  u_EnableLTC = true;
uniform float u_Roughness;
uniform float u_Intensity;
uniform float u_Metalness = 0.0;

uniform sampler2D u_LTC_MatrixTexture;
uniform sampler2D u_LTC_MagnitueTexture;
uniform sampler2D u_DiffuseTexture;

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

float DistributionGGX(vec3 vN, vec3 vH, float vRoughness)
{
	float a = vRoughness * vRoughness;
	float a2 = a * a;
	float NdotH = max(dot(vN, vH), 0.0);
	float NdotH2 = NdotH * NdotH;
	float nom = a2;  //分子
	float denom = NdotH2 * (a2 - 1.0) + 1.0;
	denom = PI * denom * denom;
	return nom / denom;		//防止vRoughness=0而且NdotH=1时出现分母除0的情况
}

float GeometrySchlickGGX(float vNdotV, float vRoughness)
{
	float r = vRoughness + 1.0;
	float k = r * r / 8.0;
	float nom = vNdotV;
	float denom = vNdotV * (1.0 - k) + k;
	return nom / denom;
}

float GeometrySmith(vec3 vN, vec3 vV, vec3 vL, float vRoughness)
{
	float NdotV = max(dot(vN, vV), 0.0);
	float NdotL = max(dot(vN, vL), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, vRoughness);
	float ggx2 = GeometrySchlickGGX(NdotL, vRoughness);
	return ggx1 * ggx2;
}

vec3 FresnelSchlick(float vCosTheta, vec3 vF0)
{
	return vF0 + (1.0 - vF0) * pow(1.0 - vCosTheta, 5.0);
}
vec3 FresnelSchlickRoughness(float vCosTheta, vec3 vF0, float vRoughness)
{
	return vF0 + (max(vec3(1.0 - vRoughness), vF0) - vF0) * pow(1.0 - vCosTheta, 5.0);
}

float DisneyDiffuse(float NdotV, float NdotL, float LdotH, float perceptualRoughness)
{
    float fd90 = 0.5 + 2 * LdotH * LdotH * perceptualRoughness;
    float lightScatter   = 1 + (fd90 - 1) * pow(1 - NdotL, 5.0);
    float viewScatter    = 1 + (fd90 - 1) * pow(1 - NdotV, 5.0);

    return lightScatter * viewScatter;
}

// Ref: http://jcgt.org/published/0003/02/03/paper.pdf
float SmithJointGGXVisibilityTerm (float NdotL, float NdotV, float roughness)
{
	float a          = roughness;
    float a2         = a * a;

    float lambdaV    = NdotL * sqrt((-NdotV * a2 + NdotV) * NdotV + a2);
    float lambdaL    = NdotV * sqrt((-NdotL * a2 + NdotL) * NdotL + a2);

    // Simplify visibility term: (2.0f * NdotL * NdotV) /  ((4.0f * NdotL * NdotV) * (lambda_v + lambda_l + 1e-5f));
    return 0.5f / (lambdaV + lambdaL + 1e-5f);
}

float GGXTerm (float NdotH, float roughness)
{
    float a2 = roughness * roughness;
    float d = (NdotH * a2 - NdotH) * NdotH + 1.0f; // 2 mad
    return 1.0 / PI * a2 / (d * d + 1e-7f);
}

vec3 FresnelTerm (vec3 F0, float cosA)
{
    float t = pow(1 - cosA, 5);   // ala Schlick interpoliation
    return F0 + (1 - F0) * t;
}

vec3 SafeNormalize(vec3 inVec)
{
    float dp3 = max(0.001f, dot(inVec, inVec));
    return inVec / sqrt(dp3);
}

float saturate(float value)
{
	return clamp(value, 0.0f, 1.0f);
}

void main()
{
	vec3 ResultColor;
	if(u_EnableLTC)
	{
		vec3 GroundNormal = normalize(v2f_Normal);	//其他几何体的话应该由其法线乘以模型矩阵来算
		vec3 ViewDir = normalize(u_CameraPosInWorldSpace - v2f_FragPosInWorldSpace);

		vec3 LightDir = normalize(u_LightPosition - v2f_FragPosInWorldSpace);
		vec3 H = normalize(ViewDir + LightDir);
		float Distance = length(u_LightPosition - v2f_FragPosInWorldSpace);	//length可以变成dot，减少开方运算
		float SpecularLightAttenuation = 1.0 / Distance;
		float DiffuseLightAttenuation = SpecularLightAttenuation / Distance;

		vec3 Albedo = texture(u_DiffuseTexture, v2f_TexCoords).rgb;
		vec3 SpecularColor;
		vec3 DiffuseColor = DiffuseAndSpecularFromMetallic (Albedo, u_Metalness, /*out*/ SpecularColor);
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
		//Diffuse *= DiffuseColor * Schlick.x + (1.0 - DiffuseColor) * Schlick.y;

		ResultColor = u_Intensity * (Diffuse * DiffuseColor/* * Kd*/ * DiffuseLightAttenuation + Specular * SpecularLightAttenuation);
		//vec3 ResultColor = u_Intensity * Diffuse * DiffuseColor * LightAttenuation * Kd;
		//vec3 ResultColor = u_Intensity * Specular * LightAttenuation;
	}
	else
	{
		vec3 GroundNormal = normalize(v2f_Normal);	//其他几何体的话应该由其法线乘以模型矩阵来算
		vec3 ViewDir = normalize(u_CameraPosInWorldSpace - v2f_FragPosInWorldSpace);

		vec3 LightDir = normalize(u_LightPosition - v2f_FragPosInWorldSpace);
		vec3 H = normalize(ViewDir + LightDir);

		//----------------original BRDF---------------------
		//float Distance = length(u_LightPosition - v2f_FragPosInWorldSpace);
		//float LightAttenuation = 1.0f / (Distance * Distance);

		//vec3 Albedo = texture(u_DiffuseTexture, v2f_TexCoords).rgb;
		//vec3 SpecularColor;
		//vec3 DiffuseColor = DiffuseAndSpecularFromMetallic (Albedo, u_Metalness, /*out*/ SpecularColor);

		//vec3 F0 = SpecularColor;
		//vec3 F = FresnelSchlickRoughness(max(dot(H, ViewDir), 0.0f), F0, u_Roughness);
		//float NDF = DistributionGGX(GroundNormal, H, u_Roughness);
		//float G = GeometrySmith(GroundNormal, ViewDir, LightDir, u_Roughness);
		//vec3 Specular = NDF * G * F / (4 * max(dot(GroundNormal, ViewDir), 0.0f) * max(dot(GroundNormal, LightDir), 0.0f) + 0.001);//0.001是为了避免除0

		//vec3 Ks = F;
		//vec3 Kd = vec3(1.0) - Ks;

		//float NormaldotLightDir = max(dot(GroundNormal, LightDir), 0.0f);
		//ResultColor = u_Intensity * (Kd * DiffuseColor /*/ PI */+ Specular/* / PI*/) * u_LightColor * LightAttenuation * NormaldotLightDir;
		//------------------BRDF in Unity---------------------
		float Smoothness = 1 - sqrt(u_Roughness);
		float Roughness = u_Roughness;
		float oneMinusReflectivity;
		vec3 SpecularColor;
		vec3 DiffuseColor = DiffuseAndSpecularFromMetallic (u_Albedo, u_Metalness, /*out*/ SpecularColor);

		//H = SafeNormalize(LightDir + ViewDir);
		float nv = abs(dot(GroundNormal, ViewDir));
		float nl = saturate(dot(GroundNormal, LightDir));
		float nh = saturate(dot(GroundNormal, H));
		float lv = saturate(dot(LightDir, ViewDir));
		float lh = saturate(dot(LightDir, H));
		// Diffuse term
	    float DiffuseTerm = DisneyDiffuse(nv, nl, lh, Roughness) * nl;
		// Specular term
		float DoubleRoughness = Roughness * Roughness;
		float V = SmithJointGGXVisibilityTerm(nl, nv, DoubleRoughness);
		float D = GGXTerm (nh, DoubleRoughness);
		float SpecularTerm = V * D * PI / 4.0; // Torrance-Sparrow model, Fresnel is applied later
		SpecularTerm = max(0, SpecularTerm * nl);
	
		float Distance = length(u_LightPosition - v2f_FragPosInWorldSpace);
		float LightAttenuation = 1.0f / ( Distance * Distance);
	    ResultColor =  (DiffuseColor * (u_LightColor * DiffuseTerm) + SpecularTerm * u_LightColor * FresnelTerm(SpecularColor, lh)) * LightAttenuation;
	}

	//vec3 ReinhardMappedColor = ResultColor / (ResultColor + vec3(1.0));
	//vec3 GammaedColor = pow(ReinhardMappedColor, vec3(1.0 / 2.2));
	vec3 GammaedColor = ResultColor;

	FragColor_ = vec4(GammaedColor, 1.0);
	//FragColor_ = vec4(1,0,0,1);
}