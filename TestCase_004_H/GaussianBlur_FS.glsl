#version 430 core

in  vec2 v2f_TexCoords;
out vec4 FragColor_;

uniform sampler2D u_Texture;
uniform bool u_IsHorizontal;
uniform int u_GaussianWeightNum;

layout (std430, binding = 1) buffer s_GaussianWeightSet
{
	float s_GaussianWeight[];
};

void main()
{
	vec2 TexelOffset = 1.0 / textureSize(u_Texture, 0); //每个纹素的大小
    vec3 Result = texture(u_Texture, v2f_TexCoords).rgb * s_GaussianWeight[0];
    if(u_IsHorizontal)
    {
        for(int i = 1; i < u_GaussianWeightNum; ++i)
        {
        Result += texture(u_Texture, v2f_TexCoords + vec2(TexelOffset.x * i, 0.0)).rgb * s_GaussianWeight[i];
        Result += texture(u_Texture, v2f_TexCoords - vec2(TexelOffset.x * i, 0.0)).rgb * s_GaussianWeight[i];
        }
    }
    else
    {
        for(int i = 1; i < u_GaussianWeightNum; ++i)
        {
            Result += texture(u_Texture, v2f_TexCoords + vec2(0.0, TexelOffset.y * i)).rgb * s_GaussianWeight[i];
            Result += texture(u_Texture, v2f_TexCoords - vec2(0.0, TexelOffset.y * i)).rgb * s_GaussianWeight[i];
        }
    }
	//Result = Result / (Result + vec3(1.0));
	//Result = pow(Result, vec3(1.0 / 2.2));

    FragColor_ = vec4(Result, 1.0);
}