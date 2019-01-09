#version 430 core
#pragma optionNV (unroll all)

in  vec3 v2f_FragPosInWorldSpace;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;
out vec4 FragColor_;

layout (std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

struct SLight{
	vec4 Position;
	vec4 Color;
};

layout (std430, binding = 1) buffer b_LightsData{
	SLight b_Lights[];
};

uniform vec3  u_CameraPosInWorldSpace;
uniform vec3  u_Albedo = vec3(1);
uniform vec4  u_DielectricSpecularColor = vec4(0.220916301, 0.220916301, 0.220916301, 1.0 - 0.220916301);
uniform vec3  u_LightColor = vec3(1);
uniform vec3  u_LightPosition;

uniform int	  u_LightNum;
uniform bool  u_EnableLTC = true;
uniform bool  u_EnableKeyK = true;
uniform float u_Roughness;
uniform float u_Intensity;
uniform float u_Metalness = 0.0;

uniform sampler2D u_LTC_MatrixTexture;
uniform sampler2D u_LTC_MagnitueTexture;
uniform sampler2D u_LTC_DisneyDiffuse_MatrixTexture;
uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_RoughnessTexture;
uniform sampler2D u_MetallicTexture;

const float PI = 3.14159265;

float integrateLTCSpecular(vec3 vFrag2Light, mat3 vLTCMatrix)
{	
	//if(u_EnableKeyK)
	//{
	//	vec3 LightPositionInTangentSpace = vLTCMatrix * vFrag2Light;
	//	if(LightPositionInTangentSpace.z < 0)
	//		return 0.0;
	//	float DistanceFromFrag2TransformedLight = length(LightPositionInTangentSpace);
	//	float JacobianDenom = pow(DistanceFromFrag2TransformedLight, 3);
	//	return LightPositionInTangentSpace.z / DistanceFromFrag2TransformedLight/* / PI*/ / JacobianDenom;
	//}
	////else
	//	//return vfrag2light.z;

	////--------------------------------------------------------------------------

	////if(u_EnableLTC)
	//else
	//{
		vec3 LightPositionInTangentSpace = vLTCMatrix * vFrag2Light;
		if(LightPositionInTangentSpace.z < 0)
			return 0.0;
		float DistanceFromFrag2TransformedLight = dot(LightPositionInTangentSpace, LightPositionInTangentSpace);		//这种要比上面那种length快一点点
		return LightPositionInTangentSpace.z / (DistanceFromFrag2TransformedLight * DistanceFromFrag2TransformedLight);
		//return 1.0;
	//}
	//else
	//	return vFrag2Light.z;
}

float integrateLTCDiffuse(vec3 vFrag2Light, mat3 vLTCMatrix, float vDistance)
{
	vec3 LightPositionInTangentSpace = vLTCMatrix * vFrag2Light;
	if(LightPositionInTangentSpace.z < 0)
		return 0.0;
	float DistanceFromFrag2Light = vDistance;
	return LightPositionInTangentSpace.z/* / DistanceFromFrag2Light*//* / PI*/;

	//vec3 LightPositionInTangentSpace = vLTCMatrix * vFrag2Light;
	//if(LightPositionInTangentSpace.z < 0)
	//	return 0.0;
	//float DistanceFromFrag2TransformedLight = dot(LightPositionInTangentSpace, LightPositionInTangentSpace);		//这种要比上面那种length快一点点
	//return LightPositionInTangentSpace.z / (DistanceFromFrag2TransformedLight * DistanceFromFrag2TransformedLight);

	//------------------------------Disney Diffuse------------------------------
	//if(u_EnableKeyK)
	//{
	//	vec3 LightPositionInTangentSpace = vLTCMatrix * vFrag2Light;
	//	if(LightPositionInTangentSpace.z < 0)
	//		return 0.0;
	//	float DistanceFromFrag2TransformedLight = length(LightPositionInTangentSpace);
	//	float JacobianDenom = pow(DistanceFromFrag2TransformedLight, 3);
	//	return LightPositionInTangentSpace.z / DistanceFromFrag2TransformedLight/* / PI*/ / JacobianDenom;
	//}
	//else
	//{
		//vec3 LightPositionInTangentSpace = vLTCMatrix * vFrag2Light;
		//if(LightPositionInTangentSpace.z < 0)
		//	return 0.0;
		//float DistanceFromFrag2TransformedLight = dot(LightPositionInTangentSpace, LightPositionInTangentSpace);		//这种要比上面那种length快一点点
		//return LightPositionInTangentSpace.z / (DistanceFromFrag2TransformedLight * DistanceFromFrag2TransformedLight);
	//}
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

//-------------------------------------Original BRDF------------------------------------------

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

//-------------------------------------Unity BRDF------------------------------------------

float DisneyDiffuse(float NdotV, float NdotL, float LdotH, float perceptualRoughness)
{
    float fd90 = 0.5 + 2 * LdotH * LdotH * perceptualRoughness;
    // Two schlick fresnel term
    float lightScatter   = 1 + (fd90 - 1) * pow(1 - NdotL, 5.0);
    float viewScatter    = 1 + (fd90 - 1) * pow(1 - NdotV, 5.0);

    return lightScatter * viewScatter;
}

float lambda(const float alpha, const float cosTheta)
{
    const float a = 1.0f / alpha / tan(acos(cosTheta));
    return (cosTheta < 1.0f) ? 0.5f * (-1.0f + sqrt(1.0f + 1.0f/a/a)) : 0.0f;    
}

// Ref: http://jcgt.org/published/0003/02/03/paper.pdf
float SmithJointGGXVisibilityTerm (float NdotL, float NdotV, float roughness)
{
	//float a          = roughness;
 //   float a2         = a * a;

 //   float lambdaV    = NdotL * sqrt((-NdotV * a2 + NdotV) * NdotV + a2);
 //   float lambdaL    = NdotV * sqrt((-NdotL * a2 + NdotL) * NdotL + a2);

 //   // Simplify visibility term: (2.0f * NdotL * NdotV) /  ((4.0f * NdotL * NdotV) * (lambda_v + lambda_l + 1e-5f));
 //   return 0.5f / (lambdaV + lambdaL + 1e-5f);

	// Original formulation: Fit code
	float a          = roughness;
	float a2         = a * a;
	float G2;
    if (NdotL <= 0.0f || NdotV <= 0.0f)
        G2 = 0;
    else
    {
		const float LambdaV = lambda(a2, NdotV);
        const float LambdaL = lambda(a2, NdotL);
        G2 = 1.0f/(1.0f + LambdaV + LambdaL);
    }
	return G2;
}

float GGXTerm (float NdotH, float roughness)
{
    float a2 = roughness * roughness;
    float d = (NdotH * a2 - NdotH) * NdotH + 1.0f; // 2 mad
    return 1.0 / PI * a2 / (d * d + 1e-7f);
}

float GGXTermFit (float NdotH, float roughness)
{
    //float a2 = roughness * roughness;
    //float d = (NdotH * a2 - NdotH) * NdotH + 1.0f; // 2 mad
    //return 1.0 / PI * a2 / (d * d + 1e-7f);

	//Fit code
	if(NdotH < 0.0)
		return 0.0;
	float a2 = roughness * roughness;
	//const vec3 H = normalize(V + L);
    //const float slopex = TdotH/NdotH;
    //const float slopey = BdotH/NdotH;
	float TanTheta = tan(acos(NdotH));
    float D = 1.0f / (1.0f + (TanTheta * TanTheta)/a2/a2);
    D = D*D;
    D = D/(3.14159f * a2*a2 * NdotH*NdotH*NdotH*NdotH + 1e-7f);
	return D;
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
	vec3 GroundNormal = normalize(v2f_Normal);	//其他几何体的话应该由其法线乘以模型矩阵来算
	vec3 ViewDir = normalize(u_CameraPosInWorldSpace - v2f_FragPosInWorldSpace);

	vec3 SpecularColor;
	vec3 Albedo = texture(u_DiffuseTexture, v2f_TexCoords).rgb;
	float Roughness = texture(u_RoughnessTexture, v2f_TexCoords).r;
	float Metallic = texture(u_MetallicTexture, v2f_TexCoords).r;
	vec3 DiffuseColor = DiffuseAndSpecularFromMetallic (u_Albedo, u_Metalness, /*out*/ SpecularColor);
	vec3 ResultColor;

	if(u_EnableLTC)
	{
		vec2 UV = LTC_Coords(dot(GroundNormal, ViewDir), u_Roughness * u_Roughness);

		vec4 LTCMatrixComponents = texture2D(u_LTC_MatrixTexture, UV);
		mat3 LTCMatrix = mat3
		(
			vec3(1, 0, LTCMatrixComponents.y),
			vec3(0, LTCMatrixComponents.z, 0),
			vec3(LTCMatrixComponents.w, 0, LTCMatrixComponents.x)
		);

		//vec4 DisneyDiffuseComponents = texture2D(u_LTC_DisneyDiffuse_MatrixTexture, UV);
		//mat3 DisneyDiffuseMatrix = mat3
		//(
		//	vec3(1, 0, DisneyDiffuseComponents.y),
		//	vec3(0, DisneyDiffuseComponents.z, 0),
		//	vec3(DisneyDiffuseComponents.w, 0, DisneyDiffuseComponents.x)
		//);

		vec3 Tangent = normalize(ViewDir - GroundNormal * dot(ViewDir, GroundNormal));
		vec3 Bitangent = cross(GroundNormal, Tangent);
		mat3 TangentSpaceInverseMatrix = transpose(mat3(Tangent, Bitangent, GroundNormal));
		mat3 LTCTangentSpaceMatrix = LTCMatrix * TangentSpaceInverseMatrix;
		//mat3 DisneyDiffuseTangentSpaceMatrix = DisneyDiffuseMatrix * TangentSpaceInverseMatrix;
	
		float JacobianNom = determinant(LTCTangentSpaceMatrix);

		vec3 Diffuse, Specular;

		//-----------------------------------------------------Multi Point Light-------------------------------------------------------------
		for(int i = 0; i < u_LightNum; ++i)
		{
			vec3 Frag2Light = b_Lights[i].Position.rgb - v2f_FragPosInWorldSpace;
			if (dot(GroundNormal, Frag2Light) < 0.0)
			{
				Diffuse  += vec3(0.0);
				Specular += vec3(0.0);
			}
			else
			{
				float DistanceFromFrag2Light = length(Frag2Light);
				Frag2Light /= DistanceFromFrag2Light;
				//float SpecularLightAttenuation = 1.0 / DistanceFromFrag2Light;
				//float DiffuseLightAttenuation = SpecularLightAttenuation / DistanceFromFrag2Light;
				float DiffuseLightAttenuation = 1.0 / DistanceFromFrag2Light / DistanceFromFrag2Light;
				Diffuse += integrateLTCDiffuse(Frag2Light, TangentSpaceInverseMatrix, DistanceFromFrag2Light) * b_Lights[i].Color.rgb * DiffuseLightAttenuation;
				//Diffuse += dot(Frag2Light, GroundNormal) * b_Lights[i].Color.rgb / (DistanceFromFrag2Light * DistanceFromFrag2Light * DistanceFromFrag2Light);	//Diffuse改成这种计算方式，速度会比用矩阵转换到切线空间要慢一些
				Specular += integrateLTCSpecular(Frag2Light, LTCTangentSpaceMatrix) * b_Lights[i].Color.rgb * DiffuseLightAttenuation;
			}
		}
		//-----------------------------------------------------Multi Point Light-------------------------------------------------------------

		////-----------------------------------------------------Single Point Light-------------------------------------------------------------
		//vec3 Frag2Light = u_LightPosition.rgb - v2f_FragPosInWorldSpace;
		//float DistanceFromFrag2Light = length(Frag2Light);
		//vec3 AttenuatedLightColor = u_LightColor.rgb / (DistanceFromFrag2Light * DistanceFromFrag2Light);
		//Diffuse += integrateLTCDiffuse(Frag2Light, TangentSpaceInverseMatrix, DistanceFromFrag2Light) * AttenuatedLightColor;
		//Specular += integrateLTCSpecular(Frag2Light, LTCTangentSpaceMatrix) * u_LightColor.rgb;
		////-----------------------------------------------------Single Point Light-------------------------------------------------------------

		Specular *= JacobianNom / PI;

		vec2 Schlick = texture2D(u_LTC_MagnitueTexture, UV).xy;
		Specular *= SpecularColor * Schlick.x + (1.0 - SpecularColor) * Schlick.y;

		ResultColor = u_Intensity * (Diffuse * DiffuseColor/* * Kd*/ + Specular)/* * LightAttenuation*/ * Albedo;
		//vec3 ResultColor = u_Intensity * Diffuse * DiffuseColor * LightAttenuation/* * Kd*/;
		//vec3 ResultColor = u_Intensity * Specular * LightAttenuation;
	}
	else
	{
		//-----------------------------Unity BRDF--------------------------------------
		float nv = abs(dot(GroundNormal, ViewDir));
		float Smoothness = 1 - sqrt(u_Roughness);
		float Roughness = u_Roughness;
		vec3 F0 = SpecularColor;

		//-----------------------------------------------------Multi Point Light-------------------------------------------------------------
		for(int i = 0; i < u_LightNum; ++i)
		{
			vec3 LightDir = b_Lights[i].Position.rgb - v2f_FragPosInWorldSpace;
			float Distance = length(LightDir);
			LightDir = normalize(LightDir);
			//vec3 H = SafeNormalize(LightDir + ViewDir);
			vec3 H = normalize(LightDir + ViewDir);
			float nl = saturate(dot(GroundNormal, LightDir));
			float nh = saturate(dot(GroundNormal, H));
			float lv = saturate(dot(LightDir, ViewDir));
			float lh = saturate(dot(LightDir, H));
			// Diffuse term
			float DiffuseTerm = /*DisneyDiffuse(nv, nl, lh, Roughness) * */nl;
			// Specular term
			float DoubleRoughness = Roughness * Roughness;
			float G = SmithJointGGXVisibilityTerm(nl, nv, DoubleRoughness);
			float D = GGXTermFit (nh, DoubleRoughness);
			float SpecularTerm = G * D/* * PI*/ / 4.0 / nv; // Torrance-Sparrow model, Fresnel is applied later
			SpecularTerm = max(0, SpecularTerm/* * nl*/);
	
			float LightAttenuation = 1.0f / ( Distance * Distance);
			ResultColor += (DiffuseColor * DiffuseTerm + SpecularTerm * FresnelTerm(SpecularColor, lh)) * b_Lights[i].Color.rgb * LightAttenuation * Albedo;

			////-----------------------------Original BRDF--------------------------------------
			//vec3 LightDir = b_Lights[i].Position.rgb - v2f_FragPosInWorldSpace;
			//float Distance = length(LightDir);
			//float LightAttenuation = 1.0 / (Distance * Distance);
			//LightDir = normalize(LightDir);
			//vec3 H = normalize(ViewDir + LightDir);

			//vec3 F = FresnelSchlickRoughness(max(dot(H, ViewDir), 0.0f), F0, u_Roughness);
			//float NDF = DistributionGGX(GroundNormal, H, u_Roughness);
			//float G = GeometrySmith(GroundNormal, ViewDir, LightDir, u_Roughness);
			//vec3  Specular = NDF * G * F / (4 * max(dot(GroundNormal, ViewDir), 0.0f) * max(dot(GroundNormal, LightDir), 0.0f) + 0.001);//0.001是为了避免除0

			//vec3 Ks = F;
			//vec3 Kd = vec3(1.0) - Ks;

			//float NormaldotLightDir = max(dot(GroundNormal, LightDir), 0.0f);
			//ResultColor += u_Intensity * (/*Kd * */DiffuseColor /*/ PI */+ Specular) * b_Lights[i].Color.rgb * LightAttenuation * NormaldotLightDir;
		}

		////-----------------------------------------------------Single Point Light-------------------------------------------------------------
		////for(int i = 0; i < u_LightNum; ++i)
		////{
		//	//vec3 LightDir = u_LightPosition.rgb - v2f_FragPosInWorldSpace;
		//	//float Distance = length(LightDir);
		//	//LightDir = normalize(LightDir);
		//	////vec3 H = SafeNormalize(LightDir + ViewDir);
		//	//vec3 H = normalize(LightDir + ViewDir);
		//	//float nl = saturate(dot(GroundNormal, LightDir));
		//	//float nh = saturate(dot(GroundNormal, H));
		//	//float lv = saturate(dot(LightDir, ViewDir));
		//	//float lh = saturate(dot(LightDir, H));
		//	//// Diffuse term
		//	//float DiffuseTerm = DisneyDiffuse(nv, nl, lh, Roughness) * nl;
		//	//// Specular term
		//	//float DoubleRoughness = Roughness * Roughness;
		//	//float V = SmithJointGGXVisibilityTerm(nl, nv, DoubleRoughness);
		//	//float D = GGXTerm (nh, DoubleRoughness);
		//	//float SpecularTerm = V * D * PI / 4.0; // Torrance-Sparrow model, Fresnel is applied later
		//	//SpecularTerm = max(0, SpecularTerm * nl);
	
		//	//float LightAttenuation = 1.0f / ( Distance * Distance);
		//	//ResultColor += (DiffuseColor * DiffuseTerm/* + SpecularTerm * FresnelTerm(SpecularColor, lh)*/) * u_LightColor.rgb * LightAttenuation;

		//	//-----------------------------Original BRDF--------------------------------------
		//	vec3 LightDir = u_LightPosition.rgb - v2f_FragPosInWorldSpace;
		//	float Distance = length(LightDir);
		//	float LightAttenuation = 1.0 / (Distance * Distance);
		//	LightDir = normalize(LightDir);
		//	vec3 H = normalize(ViewDir + LightDir);

		//	vec3 F = FresnelSchlickRoughness(max(dot(H, ViewDir), 0.0f), F0, u_Roughness);
		//	float NDF = DistributionGGX(GroundNormal, H, u_Roughness);
		//	float G = GeometrySmith(GroundNormal, ViewDir, LightDir, u_Roughness);
		//	vec3  Specular = NDF * G * F / (4 * max(dot(GroundNormal, ViewDir), 0.0f) * max(dot(GroundNormal, LightDir), 0.0f) + 0.001);//0.001是为了避免除0

		//	vec3 Ks = F;
		//	vec3 Kd = vec3(1.0) - Ks;

		//	float NormaldotLightDir = max(dot(GroundNormal, LightDir), 0.0f);
		//	ResultColor += u_Intensity * (/*Kd * */DiffuseColor /*/ PI */+ Specular) * u_LightColor.rgb * LightAttenuation * NormaldotLightDir;
		////}
		////-----------------------------------------------------Single Point Light-------------------------------------------------------------
	}

	//vec3 ReinhardMappedColor = ResultColor / (ResultColor + vec3(1.0));
	//vec3 GammaedColor = pow(ReinhardMappedColor, vec3(1.0 / 2.2));
	vec3 GammaedColor = vec3(ResultColor);
	//if(GammaedColor.r <= 0.0001 && GammaedColor.g <= 0.0001 && GammaedColor.b <= 0.0001)
	//	GammaedColor = vec3(1, 0, 0);

	FragColor_ = vec4(GammaedColor, 1.0);
}