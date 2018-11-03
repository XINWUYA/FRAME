#version 430 core

in  vec3 v2f_FragPosInWorldSpace;
in  vec2 v2f_TexCoords;
out vec4 FragColor_;

layout (std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform vec3  u_CameraPosInWorldSpace;
uniform vec3  u_DiffuseColor;
uniform vec3  u_SpecularColor;
uniform vec3  u_PolygonalLightVertexPos[4];

uniform float u_Roughness;
uniform float u_Intensity;
uniform bool  u_IsTwoSided = false;
uniform int u_Layer = 0;

uniform sampler2D u_LTC_MatrixTexture;
uniform sampler2D u_LTC_MagnitueTexture;
uniform sampler2DArray u_FilteredLightTexture;

const float LUT_SIZE = 64.0;
const float LUT_SCALE = (LUT_SIZE - 1.0) / LUT_SIZE;
const float LUT_BIAS = 0.5 / LUT_SIZE;
const float PI = 3.14159265;

void clipPolygonalLightByZPlane(inout vec3 vioPolygonalLightVertexPos[5], out int voPolygonalLightVertexNumAfterClipping)
{
	int Flag4HowManyVertexAboveZPlane = 0;
	if(vioPolygonalLightVertexPos[0].z > 0.0) Flag4HowManyVertexAboveZPlane += 1;
	if(vioPolygonalLightVertexPos[1].z > 0.0) Flag4HowManyVertexAboveZPlane += 2;
	if(vioPolygonalLightVertexPos[2].z > 0.0) Flag4HowManyVertexAboveZPlane += 4;
	if(vioPolygonalLightVertexPos[3].z > 0.0) Flag4HowManyVertexAboveZPlane += 8;

	voPolygonalLightVertexNumAfterClipping = 0;
	if(Flag4HowManyVertexAboveZPlane == 0)
	{
	}
	else if(Flag4HowManyVertexAboveZPlane == 1)
	{
		voPolygonalLightVertexNumAfterClipping = 3;
		vioPolygonalLightVertexPos[1] = -vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[0] + vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[1];
		vioPolygonalLightVertexPos[2] = -vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[0] + vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[3];
	}
	else if(Flag4HowManyVertexAboveZPlane == 2)
	{
		voPolygonalLightVertexNumAfterClipping = 3;
		vioPolygonalLightVertexPos[0] = -vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[1] + vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[0];
		vioPolygonalLightVertexPos[2] = -vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[1] + vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[2];
	}
	else if(Flag4HowManyVertexAboveZPlane == 3)
	{
		voPolygonalLightVertexNumAfterClipping = 4;
		vioPolygonalLightVertexPos[2] = -vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[1] + vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[2];
		vioPolygonalLightVertexPos[3] = -vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[0] + vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[3];
	}
	else if(Flag4HowManyVertexAboveZPlane == 4)
	{
		voPolygonalLightVertexNumAfterClipping = 3;
		vioPolygonalLightVertexPos[0] = -vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[2] + vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[3];
		vioPolygonalLightVertexPos[1] = -vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[2] + vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[1];
	}
	else if(Flag4HowManyVertexAboveZPlane == 5)
	{
		voPolygonalLightVertexNumAfterClipping = 0;
	}
	else if(Flag4HowManyVertexAboveZPlane == 6)
	{
		voPolygonalLightVertexNumAfterClipping = 4;
		vioPolygonalLightVertexPos[0] = -vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[1] + vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[0];
		vioPolygonalLightVertexPos[3] = -vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[2] + vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[3];
	}
	else if(Flag4HowManyVertexAboveZPlane == 7)
	{
		voPolygonalLightVertexNumAfterClipping = 5;
		vioPolygonalLightVertexPos[4] = -vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[0] + vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[3];
		vioPolygonalLightVertexPos[3] = -vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[2] + vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[3];
	}
	else if(Flag4HowManyVertexAboveZPlane == 8)
	{
		voPolygonalLightVertexNumAfterClipping = 3;
		vioPolygonalLightVertexPos[0] = -vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[3] + vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[0];
		vioPolygonalLightVertexPos[1] = -vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[3] + vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[2];
		vioPolygonalLightVertexPos[2] = vioPolygonalLightVertexPos[3];
	}
	else if(Flag4HowManyVertexAboveZPlane == 9)
	{
		voPolygonalLightVertexNumAfterClipping = 4;
		vioPolygonalLightVertexPos[1] = -vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[0] + vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[1];
		vioPolygonalLightVertexPos[2] = -vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[3] + vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[2];
	}
	else if(Flag4HowManyVertexAboveZPlane == 10)
	{
		voPolygonalLightVertexNumAfterClipping = 0;
	}
	else if(Flag4HowManyVertexAboveZPlane == 11)
	{
		voPolygonalLightVertexNumAfterClipping = 5;
		vioPolygonalLightVertexPos[4] = vioPolygonalLightVertexPos[3];
		vioPolygonalLightVertexPos[3] = -vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[3] + vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[2];
		vioPolygonalLightVertexPos[2] = -vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[1] + vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[2];
	}
	else if(Flag4HowManyVertexAboveZPlane == 12)
	{
		voPolygonalLightVertexNumAfterClipping = 4;
		vioPolygonalLightVertexPos[1] = -vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[2] + vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[1];
		vioPolygonalLightVertexPos[0] = -vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[3] + vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[0];
	}
	else if(Flag4HowManyVertexAboveZPlane == 13)
	{
		voPolygonalLightVertexNumAfterClipping = 5;
		vioPolygonalLightVertexPos[4] = vioPolygonalLightVertexPos[3];
		vioPolygonalLightVertexPos[3] = vioPolygonalLightVertexPos[2];
		vioPolygonalLightVertexPos[2] = -vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[2] + vioPolygonalLightVertexPos[2].z * vioPolygonalLightVertexPos[1];
		vioPolygonalLightVertexPos[1] = -vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[0] + vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[1];
	}
	else if(Flag4HowManyVertexAboveZPlane == 14)
	{
		voPolygonalLightVertexNumAfterClipping = 5;
		vioPolygonalLightVertexPos[4] = -vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[3] + vioPolygonalLightVertexPos[3].z * vioPolygonalLightVertexPos[0];
		vioPolygonalLightVertexPos[0] = -vioPolygonalLightVertexPos[0].z * vioPolygonalLightVertexPos[1] + vioPolygonalLightVertexPos[1].z * vioPolygonalLightVertexPos[0];
	}
	else if(Flag4HowManyVertexAboveZPlane == 15)
	{
		voPolygonalLightVertexNumAfterClipping = 4;
	}

	if(voPolygonalLightVertexNumAfterClipping == 3)
		vioPolygonalLightVertexPos[3] = vioPolygonalLightVertexPos[0];
	if(voPolygonalLightVertexNumAfterClipping == 4)
		vioPolygonalLightVertexPos[4] = vioPolygonalLightVertexPos[0];
}

float integrateEdge(vec3 vVertex1, vec3 vVertex2)
{
	float CosTheta = dot(vVertex1, vVertex2);
	CosTheta = clamp(CosTheta, -0.9999, 0.9999);
	float Theta = acos(CosTheta);
	return cross(vVertex1, vVertex2).z * Theta / sin(Theta);
	//return cross(vVertex1, vVertex2).z * ((Theta > 0.001) ? Theta / sin(Theta) : 1.0);

	//float x = dot(vVertex1, vVertex2);
	//float y = abs(x);
	//float a = 5.42031 + (3.12829 + 0.0902326 * y) * y;
	//float b = 3.45068 + (4.18814 + y) * y;
	//float theta_sintheta = a / b;
	//if(x < 0.0)
	//	theta_sintheta = PI * sqrt(1.0 - x*x) - theta_sintheta;
	//return theta_sintheta * cross(vVertex1, vVertex2).z;
}

vec3 integrateLTC_IL(vec3 vPolygonalLightVertexPos[5])
{
	vec3 V1 = vPolygonalLightVertexPos[1] - vPolygonalLightVertexPos[0];
	vec3 V2 = vPolygonalLightVertexPos[3] - vPolygonalLightVertexPos[0];
	/*vec3 PlaneNormal = normalize(cross(V1, V2));
	vec3 PointProjectedInPlane = -PlaneNormal * dot(vPolygonalLightVertexPos[0], PlaneNormal);

	vec3 PointProjected2P0 = PointProjectedInPlane - vPolygonalLightVertexPos[0];
	vec2 Puv;
	Puv.x = dot(PointProjected2P0, V1) / length(V1);
	Puv.y = dot(PointProjected2P0, V2) / length(V2);*/
	vec3 PlaneOrtho = (cross(V1, V2));
	float PlaneAreaSquared = dot(PlaneOrtho, PlaneOrtho);
	float PlaneDistxPlaneArea = dot(PlaneOrtho, vPolygonalLightVertexPos[0]);

	/*vec3 OrthoP1_P2 = acos(dot(vPolygonalLightVertexPos[0], vPolygonalLightVertexPos[1])) * cross(vPolygonalLightVertexPos[0], vPolygonalLightVertexPos[1]) / length(cross(vPolygonalLightVertexPos[0], vPolygonalLightVertexPos[1]));
	vec3 OrthoP2_P3 = acos(dot(vPolygonalLightVertexPos[1], vPolygonalLightVertexPos[2])) * cross(vPolygonalLightVertexPos[1], vPolygonalLightVertexPos[2]) / length(cross(vPolygonalLightVertexPos[1], vPolygonalLightVertexPos[2]));
	vec3 OrthoP3_P4 = acos(dot(vPolygonalLightVertexPos[2], vPolygonalLightVertexPos[3])) * cross(vPolygonalLightVertexPos[2], vPolygonalLightVertexPos[3]) / length(cross(vPolygonalLightVertexPos[2], vPolygonalLightVertexPos[3]));
	vec3 OrthoP4_P1 = acos(dot(vPolygonalLightVertexPos[3], vPolygonalLightVertexPos[0])) * cross(vPolygonalLightVertexPos[3], vPolygonalLightVertexPos[0]) / length(cross(vPolygonalLightVertexPos[3], vPolygonalLightVertexPos[0]));
	vec3 P_Dir = OrthoP1_P2 + OrthoP2_P3 + OrthoP3_P4 + OrthoP4_P1;
	vec3 N = normalize(PlaneOrtho);
	vec3 P = (dot(N, vPolygonalLightVertexPos[0]) / dot(N, P_Dir)) * P_Dir;*/

	vec3 P = PlaneDistxPlaneArea * PlaneOrtho / PlaneAreaSquared - vPolygonalLightVertexPos[0];

	float Dot_V1_V2 = dot(V1, V2);
	float Inv_dot_V1_V1 = 1.0 / dot(V1, V1);
	vec3 V2_ = V2 - V1 * Dot_V1_V2 * Inv_dot_V1_V1;
	vec2 Puv;
	Puv.y = dot(V2_, P) / dot(V2_, V2_);
	Puv.x = dot(V1, P)*Inv_dot_V1_V1 - Dot_V1_V2 * Inv_dot_V1_V1*Puv.y;
	/*vec2 Puv;
	Puv.x = dot(P - vPolygonalLightVertexPos[0], normalize(V1)) / length(V1);
	Puv.y = dot(P - vPolygonalLightVertexPos[0], normalize(V2)) / length(V2);*/
	//// LOD
	float Level = abs(PlaneDistxPlaneArea) / pow(PlaneAreaSquared, 0.75);

	//return texture2DLod(texLightFiltered, vec2(0.125, 0.125) + 0.75 * Puv, log(2048.0*d) / log(3.0)).rgb;

	vec3 TexCoords = vec3(/*vec2(0.125, 0.125) + 0.75 * */Puv, log(2048.0*Level) / log(3.0));
	//vec3 TexCoords = vec3(Puv, u_Layer);
	return vec3(texture(u_FilteredLightTexture, TexCoords));
	//return vec3(1.0f, 0, 0);
}


vec3 integrateLTC_ID(vec3 vNormal, vec3 vViewDir, vec3 vFragPos, mat3 vLTCMatrix, vec3 vPolygonalLightVertexPos[4], bool vTwoSided)
{	
	//着色点上的切线空间正交基
	vec3 Tangent = normalize(vViewDir - vNormal * dot(vViewDir, vNormal));
	vec3 Bitangent = cross(vNormal, Tangent);

	//将变换矩阵转换到切线空间
	vLTCMatrix = vLTCMatrix * transpose(mat3(Tangent, Bitangent, vNormal));
	//多边形顶点(因为被平面z=0裁剪以后，四边形可能变成5个顶点)
	vec3 PolygonalLightVertexPosInTangentSpace[5];
	PolygonalLightVertexPosInTangentSpace[0] = vLTCMatrix * (vPolygonalLightVertexPos[0] - vFragPos);
	PolygonalLightVertexPosInTangentSpace[1] = vLTCMatrix * (vPolygonalLightVertexPos[1] - vFragPos);
	PolygonalLightVertexPosInTangentSpace[2] = vLTCMatrix * (vPolygonalLightVertexPos[2] - vFragPos);
	PolygonalLightVertexPosInTangentSpace[3] = vLTCMatrix * (vPolygonalLightVertexPos[3] - vFragPos);
	PolygonalLightVertexPosInTangentSpace[4] = PolygonalLightVertexPosInTangentSpace[3];

	vec3 TextureLight = integrateLTC_IL(PolygonalLightVertexPosInTangentSpace);


	int PolygonalLightVertexNumAfterClipping;
	clipPolygonalLightByZPlane(PolygonalLightVertexPosInTangentSpace, PolygonalLightVertexNumAfterClipping);
	
	if(PolygonalLightVertexNumAfterClipping == 0)
		return vec3(0);
	
	//把裁剪后的多边形投影到球面上（也就是对每个顶点坐标向量归一化）
	PolygonalLightVertexPosInTangentSpace[0] = normalize(PolygonalLightVertexPosInTangentSpace[0]);
	PolygonalLightVertexPosInTangentSpace[1] = normalize(PolygonalLightVertexPosInTangentSpace[1]);
	PolygonalLightVertexPosInTangentSpace[2] = normalize(PolygonalLightVertexPosInTangentSpace[2]);
	PolygonalLightVertexPosInTangentSpace[3] = normalize(PolygonalLightVertexPosInTangentSpace[3]);
	PolygonalLightVertexPosInTangentSpace[4] = normalize(PolygonalLightVertexPosInTangentSpace[4]);

	//用累加边的公式来求解积分
	float Sum = 0.0;
	Sum += integrateEdge(PolygonalLightVertexPosInTangentSpace[0], PolygonalLightVertexPosInTangentSpace[1]);
	Sum += integrateEdge(PolygonalLightVertexPosInTangentSpace[1], PolygonalLightVertexPosInTangentSpace[2]);
	Sum += integrateEdge(PolygonalLightVertexPosInTangentSpace[2], PolygonalLightVertexPosInTangentSpace[3]);
	if(PolygonalLightVertexNumAfterClipping >= 4)
		Sum += integrateEdge(PolygonalLightVertexPosInTangentSpace[3], PolygonalLightVertexPosInTangentSpace[4]);
	if(PolygonalLightVertexNumAfterClipping == 5)
		Sum += integrateEdge(PolygonalLightVertexPosInTangentSpace[4], PolygonalLightVertexPosInTangentSpace[0]);
	Sum = vTwoSided ? abs(Sum) : max(Sum, 0.0);
	return vec3(Sum) * TextureLight;
}

void main()
{
	vec3 GroundNormal = vec3(0, 1, 0);	//其他几何体的话应该由其法线乘以模型矩阵来算
	vec3 ViewDir = normalize(u_CameraPosInWorldSpace - v2f_FragPosInWorldSpace);

	float Theta = acos(dot(GroundNormal, ViewDir));
	vec2 UV = vec2(u_Roughness, Theta / (0.5 * PI));
	UV = UV * LUT_SCALE + LUT_BIAS;

	vec4 LTCMatrixComponents = texture2D(u_LTC_MatrixTexture, UV);
	mat3 LTCMatrix = mat3
	(
		vec3(1, 0, LTCMatrixComponents.y),
		vec3(0, LTCMatrixComponents.z, 0),
		vec3(LTCMatrixComponents.w, 0, LTCMatrixComponents.x)
	);

	vec3 Diffuse = integrateLTC_ID(GroundNormal, ViewDir, v2f_FragPosInWorldSpace, mat3(1), u_PolygonalLightVertexPos, u_IsTwoSided)/* * integrateLTC_IL(u_PolygonalLightVertexPos)*/;
	vec3 Specular = integrateLTC_ID(GroundNormal, ViewDir, v2f_FragPosInWorldSpace, LTCMatrix, u_PolygonalLightVertexPos, u_IsTwoSided)/* * integrateLTC_IL(u_PolygonalLightVertexPos)*/;
	Specular *= texture2D(u_LTC_MagnitueTexture, UV).w;

	vec3 ResultColor = u_Intensity * (Diffuse * u_DiffuseColor + Specular * u_SpecularColor);
	ResultColor /= 2.0 * PI;

	FragColor_ = vec4(ResultColor, 1.0);
	//FragColor_ = vec4(integrateLTC_IL(u_PolygonalLightVertexPos),1.0f);
	//FragColor_ = texture(u_FilteredLightTexture, vec3(v2f_TexCoords, u_Layer));
}