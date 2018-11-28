#version 430 core

in vec3 v2f_WorldPos;
out vec4 Color_;

uniform samplerCube u_Cubemap;
uniform int u_Layer;

void main()
{
	//vec3 EnvColor = texture(u_Cubemap, v2f_WorldPos).rgb;
	vec3 EnvColor = textureLod(u_Cubemap, v2f_WorldPos, u_Layer).rgb;
	EnvColor = EnvColor / (EnvColor + vec3(1.0));
    EnvColor = pow(EnvColor, vec3(1.0/2.2)); 

	Color_ = vec4(EnvColor, 1.0);
}