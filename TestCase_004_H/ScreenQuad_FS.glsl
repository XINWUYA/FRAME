#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_Texture;

void main()
{
	vec3 Result = texture(u_Texture, v2f_TexCoords).rgb;
	Result = Result / (Result + vec3(1.0));
	Result = pow(Result, vec3(1.0 / 2.2));
	Color_ = vec4(Result, 1.0);
}