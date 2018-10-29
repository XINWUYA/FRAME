#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "Mesh.h"

class CShader;

class CModel
{
public:
	CModel() = default;
	CModel(const std::string &vModelPath);
	~CModel() = default;
	void update(const CShader &vShader) const;

private:
	std::vector<SMeshTexture>  m_LoadedTextures;
	std::vector<CMesh>         m_Meshes;
	std::string                m_Directory;
	const aiScene             *m_pScene;

	GLvoid __loadModel(const std::string& vPath);
	GLvoid __traverseNodes();
	CMesh  __processMesh(const aiMesh *vAiMesh);
	GLvoid __processVertex(const aiMesh *vAiMesh, std::vector<SMeshVertex> &voVertices);
	GLvoid __processIndices(const aiMesh *vAiMesh, std::vector<GLint> &voIndices);
	GLvoid __processTextures(const aiMesh *vAiMesh, std::vector<SMeshTexture> &voTextures);
	GLvoid __loadTextureFromMaterial(aiTextureType vTextureType, const aiMaterial *vMat, std::vector<SMeshTexture>& voTextures);
};