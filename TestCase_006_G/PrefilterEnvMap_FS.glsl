#version 430 core

in vec4 g2f_WorldPos;
out vec4 Color_;

uniform samplerCube u_EnvironmentCubeMap;
uniform float u_Roughness;

const float PI = 3.14159265359;

// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
// efficient VanDerCorpus calculation.
float RadicalInverse_VdC(uint uBits) 
{
     uBits = (uBits << 16u) | (uBits >> 16u);
     uBits = ((uBits & 0x55555555u) << 1u) | ((uBits & 0xAAAAAAAAu) >> 1u);
     uBits = ((uBits & 0x33333333u) << 2u) | ((uBits & 0xCCCCCCCCu) >> 2u);
     uBits = ((uBits & 0x0F0F0F0Fu) << 4u) | ((uBits & 0xF0F0F0F0u) >> 4u);
     uBits = ((uBits & 0x00FF00FFu) << 8u) | ((uBits & 0xFF00FF00u) >> 8u);
     return float(uBits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint vIndex, uint vN)
{
	return vec2(float(vIndex)/float(vN), RadicalInverse_VdC(vIndex));
}

vec3 ImportanceSampleGGX(vec2 vHammersleyVec, vec3 vNormal, float vRoughness)
{
	float a = vRoughness * vRoughness;
	//Theta��Phi�ǰ����������������
	float Phi = 2.0 * PI * vHammersleyVec.x;
	float CosTheta = sqrt((1.0 - vHammersleyVec.y) / (1.0 + (a * a - 1.0) * vHammersleyVec.y));	//����������arctan�Ĺ�ʽ�Ƴ���
	float SinTheta = sqrt(1.0 - CosTheta * CosTheta);
	//H�ǰ�������������߿ռ��������
	vec3 H;
	H.x = cos(Phi) * SinTheta;
	H.y = sin(Phi) * SinTheta;
	H.z = CosTheta;
	//�����߿ռ�ת��������ռ�
	vec3 Up        = abs(vNormal.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 Tangent   = normalize(cross(Up, vNormal));
	vec3 Bitangent = cross(vNormal, Tangent);
	vec3 SampleVec = Tangent * H.x + Bitangent * H.y + vNormal * H.z;

	return normalize(SampleVec);
}

void main()
{
	vec3 Normal = normalize(vec3(g2f_WorldPos));
	//����V=R=Normal
	vec3 R = Normal;
	vec3 V = R;
	
	const int SAMPLE_COUNT = 1024;
	vec3 PrefilteredColor = vec3(0);
	float TotalWeight = 0.0;
	
	for(int i = 0; i < SAMPLE_COUNT; ++i)
	{
		vec2 HammersleyVec = Hammersley(i, SAMPLE_COUNT);
		vec3 H = ImportanceSampleGGX(HammersleyVec, Normal, u_Roughness);
		vec3 L = normalize(2.0 * dot(V, H) * H - V);

		float NdotL = max(dot(Normal, L), 0.0);
		if(NdotL > 0.0)
		{
			PrefilteredColor += texture(u_EnvironmentCubeMap, L).rgb * NdotL;
            TotalWeight      += NdotL;
		}
	}
	PrefilteredColor = PrefilteredColor / TotalWeight;

	Color_ = vec4(PrefilteredColor, 1.0);
}