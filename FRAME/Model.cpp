#include "Model.h"
#include <iostream>
#include "Utils.h"
#include "common.h"
#include "Shader.h"

CModel::CModel(const std::string &vModelPath)
{
	__loadModel(vModelPath);
}

//************************************************************************************
//Function:
void CModel::init(const CShader &vShader) const
{
	vShader.setIntUniformValue(m_DiffuseTextureNamePrefix, 0);
	vShader.setIntUniformValue(m_SpecularTextureNamePrefix, 1);
	vShader.setIntUniformValue(m_NormalTextureNamePrefix, 2);
	vShader.setIntUniformValue(m_RoughnessTextureNamePrefix, 3);
	vShader.setIntUniformValue(m_MetallicTextureNamePrefix, 4);
}

//************************************************************************************
//Function:
GLvoid CModel::update(const CShader& vShader) const
{
	for (auto &vMesh : m_Meshes)
		vMesh.update(vShader);
}

//************************************************************************************
//Function:
GLvoid CModel::__loadModel(const std::string& vPath)
{
	Assimp::Importer ModelImpoter;
	m_pScene = ModelImpoter.ReadFile(vPath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (!m_pScene || !m_pScene->mRootNode || m_pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
	{
		std::cerr << "Error::Model:: " << ModelImpoter.GetErrorString() << std::endl;
		return;
	}
	m_Directory = vPath.substr(0, vPath.find_last_of('/'));
	__traverseNodes();
}

//************************************************************************************
//Function:
GLvoid CModel::__traverseNodes()
{
	GLint NumMeshes = static_cast<GLint>(m_pScene->mNumMeshes);
	for (GLint i = 0; i < NumMeshes; ++i)
		m_Meshes.push_back(__processMesh(m_pScene->mMeshes[i]));
}

//************************************************************************************
//Function:
CMesh CModel::__processMesh(const aiMesh *vAiMesh)
{
	_ASSERT(vAiMesh);
	std::vector<SMeshVertex> Vertices;
	std::vector<GLint> Indices;
	std::vector<SMeshTexture> Textures;
	__processVertex(vAiMesh, Vertices);
	__processIndices(vAiMesh, Indices);
	__processTextures(vAiMesh, Textures);
	return CMesh(Vertices, Indices, Textures);
}

//************************************************************************************
//Function:
GLvoid CModel::__processVertex(const aiMesh *vAiMesh, std::vector<SMeshVertex> &voVertices)
{
	_ASSERT(vAiMesh);
	GLint NumVertices = (GLint)vAiMesh->mNumVertices;
	for (GLint i = 0; i < NumVertices; ++i)
	{
		SMeshVertex Vertex;
		Vertex.Position = glm::vec3(vAiMesh->mVertices[i].x, vAiMesh->mVertices[i].y, vAiMesh->mVertices[i].z);
		Vertex.Normal = glm::vec3(vAiMesh->mNormals[i].x, vAiMesh->mNormals[i].y, vAiMesh->mNormals[i].z);
		if (vAiMesh->mTextureCoords[0]) {
			Vertex.TexCoords = glm::vec2(vAiMesh->mTextureCoords[0][i].x, vAiMesh->mTextureCoords[0][i].y);
		}
		else {
			Vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		Vertex.Tangent = glm::vec3(vAiMesh->mTangents[i].x, vAiMesh->mTangents[i].y, vAiMesh->mTangents[i].z);
		voVertices.push_back(Vertex);
	}
}

//************************************************************************************
//Function:
GLvoid CModel::__processIndices(const aiMesh *vAiMesh, std::vector<GLint> &voIndices)
{
	_ASSERT(vAiMesh);
	GLint NumFaces = vAiMesh->mNumFaces;
	for (GLint i = 0; i < NumFaces; ++i)
	{
		aiFace AiFace = vAiMesh->mFaces[i];
		GLint NumIndices = AiFace.mNumIndices;
		for (GLint k = 0; k < NumIndices; ++k)
			voIndices.push_back(AiFace.mIndices[k]);
	}
}

//************************************************************************************
//Function:
GLvoid CModel::__processTextures(const aiMesh *vAiMesh, std::vector<SMeshTexture> &voTextures)
{
	_ASSERT(vAiMesh);
	if (vAiMesh->mMaterialIndex < 0)
		return;
	aiMaterial *pAiMat = m_pScene->mMaterials[vAiMesh->mMaterialIndex];
	ElayGraphics::STexture Texture2DInSRGBSpace;
	Texture2DInSRGBSpace.isSRGBSpace = GL_TRUE;
	ElayGraphics::STexture Texture2D4Roughness;
	Texture2D4Roughness.Type4MinFilter = GL_LINEAR_MIPMAP_LINEAR;
	__loadTextureFromMaterial(aiTextureType_DIFFUSE, pAiMat, m_DiffuseTextureNamePrefix, voTextures/*, Texture2DInSRGBSpace*/);
	__loadTextureFromMaterial(aiTextureType_SPECULAR, pAiMat, m_SpecularTextureNamePrefix, voTextures);
	__loadTextureFromMaterial(aiTextureType_HEIGHT, pAiMat, m_NormalTextureNamePrefix, voTextures);
	__loadTextureFromMaterial(aiTextureType_SHININESS, pAiMat, m_RoughnessTextureNamePrefix, voTextures/*, Texture2D4Roughness*/);
	__loadTextureFromMaterial(aiTextureType_AMBIENT, pAiMat, m_MetallicTextureNamePrefix, voTextures);
}

//************************************************************************************
//Function:
GLvoid CModel::__loadTextureFromMaterial(aiTextureType vTextureType, const aiMaterial *vMat, const std::string& vTextureNamePrefix, std::vector<SMeshTexture>& voTextures, ElayGraphics::STexture Texture2D/* = ElayGraphics::STexture()*/)
{
	_ASSERT(vMat);
	GLint TextureCount = vMat->GetTextureCount(vTextureType);
	int TextureIndex = -1;
	if (TextureCount <= 0)
	{
		SMeshTexture MeshTexture;
		voTextures.push_back(MeshTexture);
		return;
	}
	for (GLint i = 0; i < TextureCount; ++i)
	{
		aiString Str;
		vMat->GetTexture(vTextureType, i, &Str);
		std::string TextureName = Str.C_Str();
		std::string TexturePath = m_Directory + "/" + TextureName;
		GLboolean Skip = GL_FALSE;
		GLint LoadedTextureCount = m_LoadedTextures.size();
		for (int k = 0; k < LoadedTextureCount; ++k)
		{
			if (TexturePath == m_LoadedTextures[k].TexturePath)
			{
				Skip = GL_TRUE;
				m_LoadedTextures[k].TextureUniformName = vTextureNamePrefix + std::to_string(++TextureIndex);
				voTextures.push_back(m_LoadedTextures[k]);
				break;
			}
		}
		if (!Skip)
		{
			SMeshTexture MeshTexture;
			MeshTexture.ID = loadTextureFromFile(TexturePath, Texture2D);
			MeshTexture.TexturePath = TexturePath;
			MeshTexture.TextureUniformName = vTextureNamePrefix + std::to_string(++TextureIndex);
			voTextures.push_back(MeshTexture);
			m_LoadedTextures.push_back(MeshTexture);
		}
	}
}