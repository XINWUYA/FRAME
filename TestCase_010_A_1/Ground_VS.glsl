#version 430 core

layout (location = 0) in vec2 _Position;
layout (location = 1) in vec2 _TexCoords;

layout (std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform mat4 u_ModelMatrix;

out vec3 v2f_FragPosInWorldSpace;
out vec2 v2f_TexCoords;
out vec3 v2f_Normal;

void main()
{
	vec4 FragPosInWorldSpace = u_ModelMatrix * vec4(_Position, 0.0, 1.0);
	v2f_FragPosInWorldSpace = vec3(FragPosInWorldSpace);
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * FragPosInWorldSpace;
	v2f_TexCoords = _TexCoords;
	v2f_Normal = mat3(transpose(inverse(u_ModelMatrix))) * vec3(0, 0, 1);
}