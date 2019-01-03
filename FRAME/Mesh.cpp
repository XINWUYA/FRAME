#include "Mesh.h"
#include "Common.h"
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
GLvoid CMesh::init(const CShader& vioShader) const
{
}

//************************************************************************************
//Function:
GLvoid CMesh::update(const CShader& vShader) const
{
	//_WARNING(m_Textures.size() > 5, "Texture num of some mesh is greater than 5.");
	for (size_t i = 0; i < m_Textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_Textures[i].ID); 
	}

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}