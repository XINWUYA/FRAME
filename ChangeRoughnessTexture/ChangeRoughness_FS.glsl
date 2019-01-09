#version 430 core

in vec2 v2f_TexCoord;
out vec4 Color_;

uniform sampler2D u_RoughnessTexture;

void main()
{
	float Roughness = texture(u_RoughnessTexture, v2f_TexCoord).r + 0.3;
	Roughness = clamp(Roughness, 0.0, 1.0);
	Color_ = vec4(vec3(Roughness), 1.0);
}