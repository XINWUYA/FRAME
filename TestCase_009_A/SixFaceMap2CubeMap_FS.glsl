#version 430 core

in vec4 g2f_WorldPos;
out vec4 Color_;

uniform sampler2D u_SphereMap;

const vec2 InvAtan = vec2(0.1591, 0.3183);

//编号0到5分别表示立方体的右、左、上、下、前、后
unsigned int calculateFaceIndexAndTexCoords(vec3 vDirection, out vec2 voTexCoords)
{
	float MaxAxis = max(max(abs(vDirection.x), abs(vDirection.y)), abs(vDirection.z));
	if(MaxAxis == abs(vDirection.x))
	{
		if(vDirection.x >= 0)
		{
			voTexCoords = vDirection.yz * 0.5 + 0.5;
			voTexCoords = voTexCoords.yx;
			voTexCoords = voTexCoords * vec2(1.0 / 3.0, 1.0 / 2.0) + vec2(2.0 / 3.0, 1.0 / 2.0);
			return 0;
		}
		else 
		{
			voTexCoords = vDirection.yz * 0.5 + 0.5;
			voTexCoords = vec2(1.0 - voTexCoords.y, voTexCoords.x);
			voTexCoords = voTexCoords * vec2(1.0 / 3.0, 1.0 / 2.0) + vec2(0.0 / 3.0, 1.0 / 2.0);
			return 1;
		}
	}
	else if(MaxAxis == abs(vDirection.y))
	{
		if(vDirection.y >= 0) 
		{
			voTexCoords = vDirection.xz * 0.5 + 0.5;
			voTexCoords = vec2(1.0 - voTexCoords.y, voTexCoords.x);
			voTexCoords = voTexCoords * vec2(1.0 / 3.0, 1.0 / 2.0) + vec2(2.0 / 3.0, 0.0);
			return 2;
		}
		else 
		{
			voTexCoords = vDirection.xz * 0.5 + 0.5;
			voTexCoords = vec2(voTexCoords.y, voTexCoords.x);
			voTexCoords = voTexCoords * vec2(1.0 / 3.0, 1.0 / 2.0) + vec2(0.0 / 3.0, 0.0);
			return 3;
		}
	}
	else if(MaxAxis == abs(vDirection.z))
	{
		if(vDirection.z >= 0)
		{
			voTexCoords = vDirection.xy * 0.5 + 0.5;
			voTexCoords = voTexCoords.yx;
			voTexCoords = voTexCoords * vec2(1.0 / 3.0, 1.0 / 2.0) + vec2(1.0 / 3.0, 0.0);
			return 4;
		}
		else
		{
			voTexCoords = vDirection.xy * 0.5 + 0.5;
			voTexCoords = voTexCoords * vec2(1.0 / 3.0, 1.0 / 2.0) + vec2(1.0 / 3.0, 1.0 / 2.0);
			return 5;
		}
	}
}


void main()
{
	vec2 UV;
	calculateFaceIndexAndTexCoords(vec3(g2f_WorldPos), UV);
	Color_ = vec4(texture(u_SphereMap, UV).rgb, 1.0);
}