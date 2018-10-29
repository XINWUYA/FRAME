#version 430 core

in vec2 v2f_TexCoord;
in vec3 v2f_Normal;
in vec3 v2f_WorldPosition;
out vec4 Color_;

uniform sampler2D u_BackDepthTexture;

uniform vec3 u_LightColor = vec3(1.0);
uniform vec3 u_ObjectColor = vec3(1,1,0);
uniform vec3 u_LightPos = vec3(0,3,0);
uniform vec2 u_ScreenSize = vec2(800, 600);
uniform vec3 u_ViewPos;
uniform vec3 u_ScatteringFactor = vec3(0.1,0.1,0.1);
uniform float u_Wrap = 0.2;
uniform float u_Near = 0.1;
uniform float u_Far = 100.0;

const float Constant = 1.0f;
const float Linear = 0.7f;
const float Quadric = 1.8f;

float linearizeDepth(float vDepth)
{
	float NDC_Z = vDepth * 2.0 - 1.0;
	return (2.0 * u_Near * u_Far) / (u_Near + u_Far - NDC_Z * (u_Far - u_Near));
}

void main()
{
	//Color_ = texture(u_DiffuseTexture0, v2f_TexCoord);
	//Color_ = vec4(1,1,0,1);
	float AmbientStrength = 0.2f;
	vec3 AmbientColor = AmbientStrength * u_ObjectColor;

	vec2 UV = gl_FragCoord.xy / u_ScreenSize;
	float BackDepth = linearizeDepth(texture(u_BackDepthTexture, UV).r);
	float FrontDepth = linearizeDepth(gl_FragCoord.z / gl_FragCoord.w);
	float DeltaDepth = BackDepth - FrontDepth;
	vec3 Scattering = exp(u_ScatteringFactor * DeltaDepth);

	vec3 Normal = normalize(v2f_Normal);
	vec3 LightDir = normalize(u_LightPos - v2f_WorldPosition);
	vec3 DiffuseColor = max((dot(Normal, LightDir) + u_Wrap) / (1 + u_Wrap), 0.0f) * u_LightColor * Scattering * u_ObjectColor;

	vec3 ViewDir = normalize(u_ViewPos - v2f_WorldPosition);
	vec3 HalfDir = normalize(LightDir + ViewDir);
	vec3 SpecularColor = pow(max(dot(HalfDir, Normal), 0.0f), 0.5) * u_LightColor * u_ObjectColor;

	Color_ = vec4(AmbientColor + DiffuseColor, 1.0f);
}