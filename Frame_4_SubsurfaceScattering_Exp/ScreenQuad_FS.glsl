#version 430 core

in vec2 v2f_TexCoord;
out vec4 Color_;

uniform sampler2D u_Texture;
uniform float u_Near = 0.1;
uniform float u_Far = 100.0;

float linearizeDepth(float vDepth)
{
	float NDC_Z = vDepth * 2.0 - 1.0;
	return (2.0 * u_Near * u_Far) / (u_Near + u_Far - NDC_Z * (u_Far - u_Near));
}

void main()
{
	//Color_ = texture(u_Texture, v2f_TexCoord);
	//Color_ = vec4(0, 1, 0, 1);
	float DepthValue = linearizeDepth(texture(u_Texture, v2f_TexCoord).r) / u_Far;
	Color_ = vec4(vec3(DepthValue), 1.0);
}