#version 430 core

in  vec2 v2f_TexCoords;
out vec4 FragColor_;

uniform sampler2D u_Image;
uniform bool u_IsHorizontal;
uniform int u_MipLevel;
uniform float u_GaussianWeight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
//uniform float u_GaussianWeight[13] = float[] (0.122386, 0.116823, 0.101585, 0.0804214, 0.0579034, 0.0378599, 0.0224355, 0.012019, 0.00580229, 0.00251432, 0.000973287, 0.000334567, 0.000101384);

void main()
{
    vec2 TexelOffset = 1.0 / textureSize(u_Image, u_MipLevel); //每个纹素的大小
    vec3 Result = textureLod(u_Image, v2f_TexCoords, u_MipLevel).rgb * u_GaussianWeight[0];
    if(u_IsHorizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
        Result += textureLod(u_Image, v2f_TexCoords + vec2(TexelOffset.x * i, 0.0), u_MipLevel).rgb * u_GaussianWeight[i];
        Result += textureLod(u_Image, v2f_TexCoords - vec2(TexelOffset.x * i, 0.0), u_MipLevel).rgb * u_GaussianWeight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            Result += textureLod(u_Image, v2f_TexCoords + vec2(0.0, TexelOffset.y * i), u_MipLevel).rgb * u_GaussianWeight[i];
            Result += textureLod(u_Image, v2f_TexCoords - vec2(0.0, TexelOffset.y * i), u_MipLevel).rgb * u_GaussianWeight[i];
        }
    }
    FragColor_ = vec4(Result, 1.0);
}