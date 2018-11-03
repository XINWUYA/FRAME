#include "PrefilterEnvMapPass.h"
#include <GLM/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Common.h"
#include "Utils.h"
#include "Interface.h"
#include "Utils.h"

CPrefilterEnvMapPass::CPrefilterEnvMapPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CPrefilterEnvMapPass::~CPrefilterEnvMapPass()
{
}

//************************************************************************************
//Function:
void CPrefilterEnvMapPass::initV()
{
	m_pShader = std::make_shared<CShader>("PrefilterEnvMap_VS.glsl", "PrefilterEnvMap_FS.glsl", "PrefilterEnvMap_GS.glsl");
	ElayGraphics::STexture PrefilterEnvTexture;
	PrefilterEnvTexture.TextureType = ElayGraphics::STexture::ETextureType::TextureCubeMap;
	PrefilterEnvTexture.InternalFormat = GL_RGB16F;
	PrefilterEnvTexture.ExternalFormat = GL_RGB;
	PrefilterEnvTexture.DataType = GL_FLOAT;
	PrefilterEnvTexture.Width = 128;
	PrefilterEnvTexture.Height = 128;
	PrefilterEnvTexture.Type4WrapR = PrefilterEnvTexture.Type4WrapS = PrefilterEnvTexture.Type4WrapT = GL_CLAMP_TO_EDGE;
	PrefilterEnvTexture.Type4MinFilter = GL_LINEAR_MIPMAP_LINEAR;
	m_PrefilterEnvMap = genTexture(PrefilterEnvTexture);
	ElayGraphics::ResourceManager::registerSharedData("PrefilterEnvMap", m_PrefilterEnvMap);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	m_FBO = ElayGraphics::ResourceManager::getSharedDataByName<int>("FBOWithCubeMapAttachment");
	m_EnvCubeMap = ElayGraphics::ResourceManager::getSharedDataByName<int>("EnvCubeMap");
	m_VPMatrices4LookAtSixFaces = ElayGraphics::ResourceManager::getSharedDataByName<std::vector<glm::mat4>>("VPMatrices4LookAtSixFaces");

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	m_pShader->activeShader();
	for (int i = 0; i < 6; ++i)
		m_pShader->setMat4UniformValue("u_VPMatrices4LookAtSixFaces[" + std::to_string(i) + "]", glm::value_ptr(m_VPMatrices4LookAtSixFaces[i]));
	m_pShader->setTextureUniformValue("u_EnvironmentCubeMap", m_EnvCubeMap, 2, GL_TEXTURE_CUBE_MAP);
	int MaxMipLevel = 5;
	for (int MipLevel = 0; MipLevel < MaxMipLevel; ++MipLevel)
	{
		GLuint MipWidth = 128 * std::pow(0.5, MipLevel);
		GLuint MipHeight = MipWidth;
		glViewport(0, 0, MipWidth, MipHeight);
		float Roughness = float(MipLevel) / (MaxMipLevel - 1);
		m_pShader->setFloatUniformValue("u_Roughness", Roughness);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_PrefilterEnvMap, MipLevel);
		glClearColor(0.01, 0.01, 0.01, 1.0);						//注意Clear要用在绑定纹理附件之后，否则因为是重用的SphereMap2CubeMapFBO，会把EnvCubeMap里的内容给Clear了
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawCube();
	}
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//************************************************************************************
//Function:
void CPrefilterEnvMapPass::updateV()
{
}