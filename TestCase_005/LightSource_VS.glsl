#version 430 core

layout (location = 0) in vec2 _Position;

layout (std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform mat4 u_ModelMatrix;

out vec4 v2f_WorldPos;

void main()
{
	v2f_WorldPos = u_ModelMatrix * vec4(_Position, 0.0, 1.0);
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(_Position, 0.0, 1.0);
}