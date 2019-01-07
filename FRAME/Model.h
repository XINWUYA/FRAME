#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "Mesh.h"
#include "Common.h"

class CShader;

class CModel
{
public:
	CModel() = default;
	CModel(const std::string &vModelPath);
	~CModel() = default;
	void init(const CShader &vShader) const;
	void update(const CShader &vShader) const;

private:
	std::vector<SMeshTexture>  m_LoadedTextures;
	std::vector<CMesh>         m_Meshes;
	std::string                m_Directory;
	std::string				   m_DiffuseTextureNamePrefix = "u_DiffuseTexture";
	std::string				   m_SpecularTextureNamePrefix = "u_SpecularTexture";
	std::string				   m_NormalTextureNamePrefix = "u_NormalTexture";
	std::string				   m_RoughnessTextureNamePrefix = "u_RoughnessTexture";
	std::string				   m_MetallicTextureNamePrefix = "u_MetallicTexture";
	const aiScene             *m_pScene;

	GLvoid __loadModel(const std::string& vPath);
	GLvoid __traverseNodes();
	CMesh  __processMesh(const aiMesh *vAiMesh);
	GLvoid __processVertex(const aiMesh *vAiMesh, std::vector<SMeshVertex> &voVertices);
	GLvoid __processIndices(const aiMesh *vAiMesh, std::vector<GLint> &voIndices);
	GLvoid __processTextures(const aiMesh *vAiMesh, std::vector<SMeshTexture> &voTextures);
	GLvoid __loadTextureFromMaterial(aiTextureType vTextureType, const aiMaterial *vMat, const std::string& vTextureNamePrefix, std::vector<SMeshTexture>& voTextures, ElayGraphics::STexture Texture2D = ElayGraphics::STexture());
};