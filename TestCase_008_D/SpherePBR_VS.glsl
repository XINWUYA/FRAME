#version 430 core

layout(location = 0) in vec3 _Position;
layout(location = 1) in vec2 _TexCoords;
layout(location = 2) in vec3 _Normal;

out vec2 v2f_TexCoords;
out vec3 v2f_FragPosInWorldSpace;
out vec3 v2f_Normal;

layout (std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};
uniform mat4 u_ModelMatrix;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(_Position, 1.0);
	v2f_TexCoords = _TexCoords;
	v2f_FragPosInWorldSpace = vec3(u_ModelMatrix * vec4(_Position, 1.0));
	v2f_Normal = mat3(u_ModelMatrix) * _Normal;
}