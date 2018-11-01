#version 430 core

layout (location=0) in vec3 _Position;

out vec3 v2f_WorldPos;

layout (std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

void main()
{
	vec4 TempPos = u_ProjectionMatrix * u_ViewMatrix * vec4(_Position,1.0f);
	//vec4 TempPos = u_ProjectionMatrix * mat4(mat3(u_ViewMatrix)) * vec4(_Position,1.0f);
	gl_Position = TempPos.xyzw;
	v2f_WorldPos = _Position;
}