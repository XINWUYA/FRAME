#include "SixFaceMap2CubeMapPass.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Interface.h"
#include "Utils.h"

CSixFaceMap2CubeMapPass::CSixFaceMap2CubeMapPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CSixFaceMap2CubeMapPass::~CSixFaceMap2CubeMapPass()
{
}

//************************************************************************************
//Function:
void CSixFaceMap2CubeMapPass::initV()
{
	m_HDRTexture = loadTextureFromFile("../Textures/hdr/Room.jpg");
	m_CubeVAO = ElayGraphics::ResourceManager::getOrCreateCubeVAO();
	m_pShader = std::make_shared<CShader>("SixFaceMap2CubeMap_VS.glsl", "SixFaceMap2CubeMap_FS.glsl", "SixFaceMap2CubeMap_GS.glsl");

	ElayGraphics::STexture CubeTexture;
	CubeTexture.TextureType = ElayGraphics::STexture::ETextureType::TextureCubeMap;
	CubeTexture.InternalFormat = GL_RGB16F;
	CubeTexture.ExternalFormat = GL_RGB;
	CubeTexture.DataType = GL_FLOAT;
	CubeTexture.Width = m_FaceSize;
	CubeTexture.Height = m_FaceSize;
	CubeTexture.Type4WrapR = CubeTexture.Type4WrapS = CubeTexture.Type4WrapT = GL_CLAMP_TO_EDGE;
	//CubeTexture.isMipmap = GL_TRUE;
	m_EnvCubeMap = genTexture(CubeTexture);

	ElayGraphics::ResourceManager::registerSharedData("EnvCubeMap", m_EnvCubeMap);

	m_FBO = genFBO({ CubeTexture });

	GLfloat EnvCubeNearPlane = 0.1f, EnvCubeFarPlane = 25.0f;
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(90.0f), GLfloat(m_FaceSize) / m_FaceSize, EnvCubeNearPlane, EnvCubeFarPlane);
	m_VPMatrices4LookAtSixFaces.push_back(ProjectionMatrix * glm::lookAt(glm::vec3(0), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)));
	m_VPMatrices4LookAtSixFaces.push_back(ProjectionMatrix * glm::lookAt(glm::vec3(0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));
	m_VPMatrices4LookAtSixFaces.push_back(ProjectionMatrix * glm::lookAt(glm::vec3(0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)));
	m_VPMatrices4LookAtSixFaces.push_back(ProjectionMatrix * glm::lookAt(glm::vec3(0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)));
	m_VPMatrices4LookAtSixFaces.push_back(ProjectionMatrix * glm::lookAt(glm::vec3(0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)));
	m_VPMatrices4LookAtSixFaces.push_back(ProjectionMatrix * glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)));

	/*glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glViewport(0, 0, 512, 512);
	m_pShader->activeShader();
	for (int i = 0; i < 6; ++i)
		m_pShader->setMat4UniformValue("u_VPMatrices4LookAtSixFaces[" + std::to_string(i) + "]", glm::value_ptr(m_VPMatrices4LookAtSixFaces[i]));
	glBindTexture(GL_TEXTURE_2D, m_HDRTexture);
	glBindVertexArray(m_CubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

	/*glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubeMap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);*/
}

//************************************************************************************
//Function:
void CSixFaceMap2CubeMapPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glViewport(0, 0, m_FaceSize, m_FaceSize);
	m_pShader->activeShader();
	for (int i = 0; i < 6; ++i)
		m_pShader->setMat4UniformValue("u_VPMatrices4LookAtSixFaces[" + std::to_string(i) + "]", glm::value_ptr(m_VPMatrices4LookAtSixFaces[i]));
	glBindTexture(GL_TEXTURE_2D, m_HDRTexture);
	glBindVertexArray(m_CubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}