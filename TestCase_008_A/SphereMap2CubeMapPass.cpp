#include "SphereMap2CubeMapPass.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Interface.h"
#include "Utils.h"

CSphereMap2CubeMapPass::CSphereMap2CubeMapPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CSphereMap2CubeMapPass::~CSphereMap2CubeMapPass()
{
}

//************************************************************************************
//Function:
void CSphereMap2CubeMapPass::initV()
{
	m_InputSphericalTexture = loadTextureFromFile("../Textures/hdr/newport_loft_blured.jpg");
	//m_InputSphericalTexture = ElayGraphics::ResourceManager::getSharedDataByName<int>("BluredTexture");
	m_CubeVAO = ElayGraphics::ResourceManager::getOrCreateCubeVAO();
	m_MaxMipMapLevel = ElayGraphics::ResourceManager::getSharedDataByName<int>("MaxMipLevel");
	m_pShader = std::make_shared<CShader>("SphereMap2CubeMap_VS.glsl", "SphereMap2CubeMap_FS.glsl", "SphereMap2CubeMap_GS.glsl");

	ElayGraphics::STexture CubeTexture;
	CubeTexture.TextureType = ElayGraphics::STexture::ETextureType::TextureCubeMap;
	CubeTexture.InternalFormat = GL_RGB16F;
	CubeTexture.ExternalFormat = GL_RGB;
	CubeTexture.DataType = GL_FLOAT;
	CubeTexture.Width = 512;  //128
	CubeTexture.Height = 512;
	CubeTexture.Type4WrapR = CubeTexture.Type4WrapS = CubeTexture.Type4WrapT = GL_CLAMP_TO_EDGE;
	CubeTexture.isMipmap = GL_TRUE;
	m_EnvCubeMap = genTexture(CubeTexture);
	ElayGraphics::ResourceManager::registerSharedData("EnvCubeMap", m_EnvCubeMap);

	m_FBO = genFBO({ CubeTexture });
	ElayGraphics::ResourceManager::registerSharedData("FBOWithCubeMapAttachment", m_FBO);	//这个帧缓存可以被下一个pass（卷积环境贴图）重复使用

	GLfloat EnvCubeNearPlane = 0.1f, EnvCubeFarPlane = 25.0f;
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(90.0f), GLfloat(512) / 512, EnvCubeNearPlane, EnvCubeFarPlane);
	m_VPMatrices4LookAtSixFaces.push_back(ProjectionMatrix * glm::lookAt(glm::vec3(0), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)));
	m_VPMatrices4LookAtSixFaces.push_back(ProjectionMatrix * glm::lookAt(glm::vec3(0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));
	m_VPMatrices4LookAtSixFaces.push_back(ProjectionMatrix * glm::lookAt(glm::vec3(0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)));
	m_VPMatrices4LookAtSixFaces.push_back(ProjectionMatrix * glm::lookAt(glm::vec3(0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)));
	m_VPMatrices4LookAtSixFaces.push_back(ProjectionMatrix * glm::lookAt(glm::vec3(0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)));
	m_VPMatrices4LookAtSixFaces.push_back(ProjectionMatrix * glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)));
	ElayGraphics::ResourceManager::registerSharedData("VPMatrices4LookAtSixFaces", m_VPMatrices4LookAtSixFaces);	//可以被后面需要渲染立方体六个面的pass重复使用

	//glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//m_pShader->activeShader();
	//for (int i = 0; i < 6; ++i)
	//	m_pShader->setMat4UniformValue("u_VPMatrices4LookAtSixFaces[" + std::to_string(i) + "]", glm::value_ptr(m_VPMatrices4LookAtSixFaces[i]));
	////glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubeMap);
	//m_pShader->setTextureUniformValue("u_SphereMap", m_InputSphericalTexture, 0);
	//for (int MipLevel = 0; MipLevel < m_MaxMipMapLevel; ++MipLevel)
	//{
	//	GLuint MipWidth = 128 * std::pow(0.5, MipLevel);
	//	GLuint MipHeight = MipWidth;
	//	glViewport(0, 0, MipWidth, MipHeight);
	//	//float Roughness = float(MipLevel) / (m_MaxMipMapLevel - 1);
	//	m_pShader->setFloatUniformValue("u_MipmapLevel", MipLevel);
	//	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_EnvCubeMap, MipLevel);
	//	glClearColor(0.01, 0.01, 0.01, 1.0);						//注意Clear要用在绑定纹理附件之后，否则因为是重用的SphereMap2CubeMapFBO，会把EnvCubeMap里的内容给Clear了
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	drawCube();
	//}
	//glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//************************************************************************************
//Function:
void CSphereMap2CubeMapPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	m_pShader->activeShader();
	for (int i = 0; i < 6; ++i)
		m_pShader->setMat4UniformValue("u_VPMatrices4LookAtSixFaces[" + std::to_string(i) + "]", glm::value_ptr(m_VPMatrices4LookAtSixFaces[i]));
	//glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubeMap);
	m_pShader->setTextureUniformValue("u_SphereMap", m_InputSphericalTexture, 0);
	for (int MipLevel = 0; MipLevel < m_MaxMipMapLevel; ++MipLevel)
	{
		GLuint MipWidth = 512 * std::pow(0.5, MipLevel);	//128
		GLuint MipHeight = MipWidth;
		glViewport(0, 0, MipWidth, MipHeight);
		//float Roughness = float(MipLevel) / (m_MaxMipMapLevel - 1);
		m_pShader->setFloatUniformValue("u_MipmapLevel", MipLevel);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_EnvCubeMap, MipLevel);
		glClearColor(0.01, 0.01, 0.01, 1.0);						//注意Clear要用在绑定纹理附件之后，否则因为是重用的SphereMap2CubeMapFBO，会把EnvCubeMap里的内容给Clear了
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawCube();
	}
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}