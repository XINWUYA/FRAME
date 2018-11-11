#version 430 core

in  vec2 v2f_TexCoords;
out vec4 Color_;

uniform sampler2D u_BluredTexture;
uniform int u_Layer = 0;

void main()
{
	//Color_ = texture(u_BluredTexture, v2f_TexCoords);
	Color_ = textureLod(u_BluredTexture, v2f_TexCoords, 7);
	//Color_ = vec4(0, 1, 0, 1);
}