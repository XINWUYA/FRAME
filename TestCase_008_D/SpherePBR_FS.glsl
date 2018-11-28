#version 430 core

in vec2 v2f_TexCoords;
in vec3 v2f_FragPosInWorldSpace;
in vec3 v2f_Normal;
out vec4 Color_;

uniform sampler2D u_LTC_MatrixTexture;
uniform sampler2D u_LTC_MagnitueTexture;
uniform sampler2D u_BRDFMap;
uniform samplerCube u_FilteredEnvMap;
uniform vec3  u_DiffuseColor;
uniform vec3  u_SpecularColor = vec3(1);
uniform float u_Metalness;
uniform float u_Roughness;
uniform float u_Intensity = 1.0;
uniform float u_AO;
uniform vec3  u_LightPos[4];
uniform vec3  u_LightColors[4];
uniform vec3  u_ViewPos;

const float PI = 3.14159265359;

//正态分布函数，用于估算微平面取向
float DistributionGGX(vec3 vN, vec3 vH, float vRoughness)
{
	float a = vRoughness * vRoughness;
	float a2 = a * a;
	float NdotH = max(dot(vN, vH), 0.0);
	float NdotH2 = NdotH * NdotH;
	float nom = a2;  //分子
	float denom = NdotH2 * (a2 - 1.0) + 1.0;
	denom = PI * denom * denom;
	return nom / max(denom, 0.001);		//防止vRoughness=0而且NdotH=1时出现分母除0的情况
}

float GeometrySchlickGGX(float vNdotV, float vRoughness)
{
	float r = vRoughness + 1.0;
	float k = r * r / 8.0;
	float nom = vNdotV;
	float denom = vNdotV * (1.0 - k) + k;
	return nom / denom;
}

//几何函数，用于描述微平面的几何遮蔽和几何阴影
float GeometrySmith(vec3 vN, vec3 vV, vec3 vL, float vRoughness)
{
	float NdotV = max(dot(vN, vV), 0.0);
	float NdotL = max(dot(vN, vL), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, vRoughness);
	float ggx2 = GeometrySchlickGGX(NdotL, vRoughness);
	return ggx1 * ggx2;
}

//菲涅尔方程，用于描述反射光线比例
vec3 FresnelSchlick(float vCosTheta, vec3 vF0)
{
	return vF0 + (1.0 - vF0) * pow(1.0 - vCosTheta, 5.0);
}
//考虑上粗糙度的菲涅尔方程
vec3 FresnelSchlickRoughness(float vCosTheta, vec3 vF0, float vRoughness)
{
	return vF0 + (max(vec3(1.0 - vRoughness), vF0) - vF0) * pow(1.0 - vCosTheta, 5.0);
}

vec3 fecthFilteredEnvMap(vec3 vLooupVector, float vRoughness)
{
	vec3 UV = vLooupVector;		//直接用法线向量来采过滤后的环境贴图是否正确？	

	const float MAX_MIPMAP_LEVEL = 6.0;

	float Lod = vRoughness * MAX_MIPMAP_LEVEL;	//这里的LOD怎么算？
	//Lod = min(pow(Lod, 1.0/2.0) * 3, 6.0);
	//Lod = -0.3902*pow(vRoughness,4) + 3.248*pow(vRoughness,3) - 9.869*pow(vRoughness,2) + 12.91*vRoughness + 0.3992;

	return textureLod(u_FilteredEnvMap, UV, Lod).rgb;
}

vec3 fecthFilteredEnvMap4Diffuse(vec3 vLooupVector)
{
	vec3 UV = vLooupVector;		//直接用法线向量来采过滤后的环境贴图是否正确？	

	const float MAX_MIPMAP_LEVEL = 6.0;

	return textureLod(u_FilteredEnvMap, UV, MAX_MIPMAP_LEVEL).rgb;
}

vec3 integrateLTC(vec3 vNormal, vec3 vViewDir, vec3 vFragPos, mat3 vLTCMatrix, vec3 vReflectionDir)
{	
	vec3 VSum = vec3(1.0);		//这个地方应该要算球面转换后被截掉的比例 

	float CosThetaBetweenHemiSphere = dot(normalize(vNormal), normalize(vLTCMatrix * vNormal));
	VSum = vec3((1 + CosThetaBetweenHemiSphere) / 2.0);
 
	vec3 TransformN = normalize(vLTCMatrix * vReflectionDir);
	vec3 TextureLight = vLTCMatrix == mat3(1) ? fecthFilteredEnvMap4Diffuse(vNormal) : fecthFilteredEnvMap(vReflectionDir, u_Roughness);

	return VSum * TextureLight;
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
	vec3 N = normalize(v2f_Normal);
	vec3 V = normalize(u_ViewPos - v2f_FragPosInWorldSpace);
	vec3 R = reflect(-V, N); 

	//计算基础反射率
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, u_DiffuseColor, u_Metalness);
	
	//反射率方程
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < 4; ++i)
	{
		//计算光源的入射辐射度Li
		vec3 L = normalize(u_LightPos[i] - v2f_FragPosInWorldSpace);
		vec3 H = normalize(V + L);
		float Distance = length(u_LightPos[i] - v2f_FragPosInWorldSpace);
		float Attenuation = 1.0 / (Distance * Distance);
		vec3 Radiance = u_LightColors[i] * Attenuation;

		//计算Cook-Torrance BRDF
		float D = DistributionGGX(N, H, u_Roughness);
		vec3  F = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
		float G = GeometrySmith(N, V, L, u_Roughness);
		vec3 nom = D * F * G;
		float denom = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 Specular = nom / max(denom, 0.001);	//防止NdotV=0.0或者NdotL=0.0时，分母除0
		
		//计算漫反射和镜面反射的比例
		vec3 ks = F;
		vec3 kd = vec3(1.0) - ks;
		kd *= (1.0 - u_Metalness);	//金属表面没有漫反射
		
		float NdotL = max(dot(N, L), 0.0);
		Lo += (kd * u_DiffuseColor / PI + Specular) * Radiance * NdotL;
	}
	
	//vec3 Ambient = vec3(0.03) * u_DiffuseColor * u_AO;
	vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, u_Roughness);
	vec3 Ks = F;
	vec3 Kd = 1.0 - Ks;
	Kd *= 1.0 - u_Metalness;
	//Kd = vec3(1.0);
	//vec3 Irradiance = texture(u_IrradianceMap, N).rgb;
	//vec3 DiffusePart = Irradiance * u_DiffuseColor * u_AO;
	
	//const float MAX_REFLECTION_LOD = 4.0;
 //   vec3 PrefilteredColor = textureLod(u_PrefilterEnvMap, R,  u_Roughness * MAX_REFLECTION_LOD).rgb;    
 //   vec2 BRDFColor = texture(u_BRDFMap, vec2(max(dot(N, V), 0.0), u_Roughness)).rg;
 //   vec3 SpecularPart = PrefilteredColor * (F * BRDFColor.x + BRDFColor.y);

	//vec3 Ambient = Kd * DiffusePart + SpecularPart;

	vec2 UV = LTC_Coords(dot(N, V), u_Roughness);
	vec4 LTCMatrixComponents = texture2D(u_LTC_MatrixTexture, UV);
	mat3 LTCMatrix = mat3
	(
		vec3(1, 0, LTCMatrixComponents.y),
		vec3(0, LTCMatrixComponents.z, 0),
		vec3(LTCMatrixComponents.w, 0, LTCMatrixComponents.x)
	);
	
	//vec3 Diffuse = integrateLTC(N, V, v2f_FragPosInWorldSpace, mat3(1), R);
	//Diffuse *= Kd;
	//vec3 TransformN = normalize(LTCMatrix * R);
	//vec3 Specular = integrateLTC(N, V, v2f_FragPosInWorldSpace, LTCMatrix, R);
	//vec2 Schlick = texture2D(u_LTC_MagnitueTexture, UV).xy;
	//Specular *= u_SpecularColor * Schlick.x + (1.0 - u_SpecularColor) * Schlick.y;

	//vec3 AmbientColor = u_Intensity * (Diffuse * u_DiffuseColor + Specular);
	//vec3 AmbientColor = u_Intensity * Diffuse * u_DiffuseColor;
	//vec3 AmbientColor = u_Intensity * Specular;
	//AmbientColor /= 2.0 * PI;

	//---------------------使用BRDFMap-------------------------
	const float MAX_MIPMAP_LEVEL = 6.0;
	vec3 Diffuse = textureLod(u_FilteredEnvMap, N, MAX_MIPMAP_LEVEL).rgb;
	Diffuse *= Kd;
	vec3 TransformN = normalize(LTCMatrix * R);
	//vec3 Specular = integrateLTC(N, V, v2f_FragPosInWorldSpace, LTCMatrix, R);
	vec3 Specular = textureLod(u_FilteredEnvMap, R,  u_Roughness * MAX_MIPMAP_LEVEL).rgb;
    vec2 BRDFColor = texture(u_BRDFMap, vec2(max(dot(N, V), 0.0), u_Roughness)).rg;
	Specular = Specular * (F * BRDFColor.x + BRDFColor.y);
	//---------------------------------------------------------

	vec3 AmbientColor = u_Intensity * (Diffuse * u_DiffuseColor + Specular);
	//vec3 AmbientColor = u_Intensity * Specular;
	//vec3 AmbientColor = u_Intensity * Diffuse;

	vec3 Color = AmbientColor/* + Lo*/;

	//HDR色调映射
	Color = Color / (Color + vec3(1.0));
	//Gamma校正
	Color = pow(Color, vec3(1.0 / 2.2));

	Color_ = vec4(Color, 1.0);
}