#include "Mesh.h"
#include "Utils.h"
#include "Shader.h"

CMesh::CMesh(std::vector<SMeshVertex> vVertices, std::vector<GLint> vIndices, std::vector<SMeshTexture> vTexture)
	:m_Vertices(vVertices), m_Indices(vIndices), m_Textures(vTexture)
{
	__init();
}

CMesh::~CMesh()
{
}

//************************************************************************************
//Function:
GLvoid CMesh::__init()
{
	m_VAO = createVAO(&m_Vertices[0], m_Vertices.size() * sizeof(SMeshVertex), { 3,3,2 }, &m_Indices[0], m_Indices.size() * sizeof(GLint));
}

//************************************************************************************
//Function:
GLvoid CMesh::update(const CShader& vShader) const
{
	vShader.activeShader();
	int i = -1;
	for (auto &vTexture : m_Textures)
	{
		vShader.setTextureUniformValue("u_DiffuseTexture" + std::to_string(++i), vTexture.ID, i);
	}
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}