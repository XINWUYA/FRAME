#version 430 core

in vec3 v2f_WorldPos;
out vec4 Color_;

uniform sampler2D u_SphereMap;

const vec2 InvAtan = vec2(0.1591, 0.3183);  //0.1591 = 1 / (2*PI); 0.3183 = 1 / (PI)

vec2 sampleSphereMap(vec3 v)
{
	vec2 UV = vec2(atan(v.z, v.x), asin(v.y));
	UV *= InvAtan;
	UV += 0.5;
	return UV;
}

void main()
{
	vec2 UV = sampleSphereMap(normalize(v2f_WorldPos));
	Color_ = vec4(texture(u_SphereMap, UV).rgb, 1.0);
}