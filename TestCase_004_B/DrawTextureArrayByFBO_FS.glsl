#version 430 core

in  vec2 v2f_TexCoords;
layout (location = 0) out vec4 Color0_;
layout (location = 1) out vec4 Color1_;
layout (location = 2) out vec4 Color2_;
layout (location = 3) out vec4 Color3_;
layout (location = 4) out vec4 Color4_;

uniform sampler2DArray u_TextureArray;

void main()
{
	Color0_ = texture(u_TextureArray, vec3(v2f_TexCoords, 0)) * vec4(1, 0, 0, 1);
	Color1_ = texture(u_TextureArray, vec3(v2f_TexCoords, 1)) * vec4(0, 1, 0, 1);
	Color2_ = texture(u_TextureArray, vec3(v2f_TexCoords, 2)) * vec4(0, 0, 1, 1);
	Color3_ = texture(u_TextureArray, vec3(v2f_TexCoords, 3)) * vec4(1, 1, 0, 1);
	Color4_ = texture(u_TextureArray, vec3(v2f_TexCoords, 4)) * vec4(0, 1, 1, 1);
}