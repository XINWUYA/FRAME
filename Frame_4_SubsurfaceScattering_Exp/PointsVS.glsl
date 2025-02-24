#version 430 core
layout (location=0) in vec3 _Position;
layout (location=1) in vec3 _Normal;
layout (location=2) in vec2 _TexCoord;

layout (std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
	mat4 u_ModelMatrix;
};

out vec2 v2f_TexCoord;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(_Position,1.0f);
	v2f_TexCoord = _TexCoord;
}