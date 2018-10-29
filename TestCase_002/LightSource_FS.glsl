#version 430 core

in  vec4 v2f_WorldPos;
out vec4 Color_;

uniform float u_Intensity = 1.0;
uniform vec3 LightSourceColor = vec3(1, 1, 1);

void main()
{
	Color_ = vec4(u_Intensity * LightSourceColor, 1.0);
}