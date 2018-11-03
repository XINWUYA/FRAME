#version 430 core

in  vec2 v2f_TexCoord;
out vec4 Color_;

uniform sampler2D u_Texture;

void main()
{
	Color_ = texture(u_Texture, vec2(0.2, 0.2) + v2f_TexCoord);
}