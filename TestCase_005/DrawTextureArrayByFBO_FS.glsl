#version 430 core

in  vec2 v2f_TexCoords;
layout (location = 0) out vec4 Color0_;
layout (location = 1) out vec4 Color1_;
layout (location = 2) out vec4 Color2_;
layout (location = 3) out vec4 Color3_;
layout (location = 4) out vec4 Color4_;

uniform sampler2DArray u_TextureArray;
uniform bool u_IsHorizontal;

uniform int uGaussianRadiusArray[5] = { 0, 1, 2, 7, 20 };
uniform float uWeightArray[5][21] = {
	{1.0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.4, 0.3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.294118, 0.235294, 0.117647, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.185497, 0.166948, 0.121417, 0.0708263, 0.0326891, 0.0116747, 0.00311324, 0.000583733, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.119604, 0.114404, 0.100103, 0.0800828, 0.058522, 0.0390147, 0.0236875, 0.013069, 0.00653448, 0.00295106, 0.00119887, 0.000435951, 0.000141043, 4.0298e-05, 1.00745e-05, 2.17827e-06, 4.01261e-07, 6.17324e-08, 7.71655e-09, 7.52834e-10, 5.37739e-11}
};

vec4 getColorByRadius(int vTextureIndex)
{
	vec3 TextureOffset = 1.0f / textureSize(u_TextureArray, 0);

	vec3 ResultColor = texture(u_TextureArray, vec3(v2f_TexCoords, vTextureIndex)).rgb * uWeightArray[vTextureIndex][0];
	if (u_IsHorizontal)
	{
		for (int i = 1; i <= uGaussianRadiusArray[vTextureIndex]; ++i)
		{
			ResultColor += texture(u_TextureArray, vec3(v2f_TexCoords, vTextureIndex) + vec3(TextureOffset.x * i, 0.0f, 0.0f)).rgb * uWeightArray[vTextureIndex][i];
			ResultColor += texture(u_TextureArray, vec3(v2f_TexCoords, vTextureIndex) - vec3(TextureOffset.x * i, 0.0f, 0.0f)).rgb * uWeightArray[vTextureIndex][i];
		}
	}
	else
	{
		for (int i = 1; i <= uGaussianRadiusArray[vTextureIndex]; ++i)
		{
			ResultColor += texture(u_TextureArray, vec3(v2f_TexCoords, vTextureIndex) + vec3(0.0f, TextureOffset.y * i, 0.0f)).rgb * uWeightArray[vTextureIndex][i];
			ResultColor += texture(u_TextureArray, vec3(v2f_TexCoords, vTextureIndex) - vec3(0.0f, TextureOffset.y * i, 0.0f)).rgb * uWeightArray[vTextureIndex][i];
		}
	}

	return vec4(ResultColor, 1.0f);
}

void main()
{
	/*Color0_ = texture(u_TextureArray, vec3(v2f_TexCoords, 0)) * vec4(1, 0, 0, 1);
	Color1_ = texture(u_TextureArray, vec3(v2f_TexCoords, 1)) * vec4(0, 1, 0, 1);
	Color2_ = texture(u_TextureArray, vec3(v2f_TexCoords, 2)) * vec4(0, 0, 1, 1);
	Color3_ = texture(u_TextureArray, vec3(v2f_TexCoords, 3)) * vec4(1, 1, 0, 1);
	Color4_ = texture(u_TextureArray, vec3(v2f_TexCoords, 4)) * vec4(0, 1, 1, 1);*/
	Color0_ = getColorByRadius(0);
	Color1_ = getColorByRadius(1);
	Color2_ = getColorByRadius(2);
	Color3_ = getColorByRadius(3);
	Color4_ = getColorByRadius(4);
}