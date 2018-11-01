#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2DArray u_TextureArray;
uniform int u_Layer = 0;

void main()
{
	Color_ = texture(u_TextureArray, vec3(v2f_TexCoords, u_Layer));
}