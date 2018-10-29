#version 430 core

in vec2 v2f_TexCoord;
out vec4 Color_;

uniform sampler2D u_DiffuseTexture0;

void main()
{
	Color_ = texture(u_DiffuseTexture0, v2f_TexCoord);
}