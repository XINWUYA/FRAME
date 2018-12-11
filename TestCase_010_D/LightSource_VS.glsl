#version 430 core

layout (location = 0) in vec3 _Position;

layout (std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(_Position, 1.0);
	gl_PointSize = 5.0;
}