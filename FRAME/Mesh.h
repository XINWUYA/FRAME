#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <string>
#include <vector>

class CShader;

struct SMeshVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	SMeshVertex() {}
};

struct SMeshTexture
{
	GLint ID = -1;
	std::string TexturePath;
	std::string TextureUniformName;

	SMeshTexture() {}
};

class CMesh
{
public:
	CMesh() = default;
	CMesh(std::vector<SMeshVertex> vVertices, std::vector<GLint> vIndices, std::vector<SMeshTexture> vTexture);
	~CMesh();
	GLvoid init(const CShader& vioShader) const;
	GLvoid update(const CShader& vShader) const;

private:
	std::vector<SMeshVertex> m_Vertices;
	std::vector<SMeshTexture> m_Textures;
	std::vector<GLint> m_Indices;
	GLint m_VAO;

	GLvoid __init();
};