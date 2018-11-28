#version 430 core

in  vec2 v2f_TexCoords;
out vec4 FragColor_;

uniform sampler2D u_InputImage;
uniform bool u_IsHorizontal;
uniform int u_MipLevel;
uniform float u_GaussianWeight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
	vec3 Result = vec3(0.0);
	if (u_MipLevel == 0)
		Result = textureLod(u_InputImage, v2f_TexCoords, u_MipLevel).rgb;
	else
	{
		vec2 TexelOffset = 1.0 / textureSize(u_InputImage, u_MipLevel); //每个纹素的大小
		Result = textureLod(u_InputImage, v2f_TexCoords, u_MipLevel).rgb * u_GaussianWeight[0];
		if (u_IsHorizontal)
		{
			for (int i = 1; i < 5; ++i)
			{
				Result += textureLod(u_InputImage, v2f_TexCoords + vec2(TexelOffset.x * i, 0.0), u_MipLevel).rgb * u_GaussianWeight[i];
				Result += textureLod(u_InputImage, v2f_TexCoords - vec2(TexelOffset.x * i, 0.0), u_MipLevel).rgb * u_GaussianWeight[i];
			}
		}
		else
		{
			for (int i = 1; i < 5; ++i)
			{
				Result += textureLod(u_InputImage, v2f_TexCoords + vec2(0.0, TexelOffset.y * i), u_MipLevel).rgb * u_GaussianWeight[i];
				Result += textureLod(u_InputImage, v2f_TexCoords - vec2(0.0, TexelOffset.y * i), u_MipLevel).rgb * u_GaussianWeight[i];
			}
		}
	}
    FragColor_ = vec4(Result, 1.0);
	//FragColor_ = vec4(0, 1, 0, 1);
}