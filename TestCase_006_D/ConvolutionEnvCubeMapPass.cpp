#include "ConvolutionEnvCubeMapPass.h"
#include <GLM/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Utils.h"
#include "Interface.h"

CConvolutionEnvCubeMapPass::CConvolutionEnvCubeMapPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CConvolutionEnvCubeMapPass::~CConvolutionEnvCubeMapPass()
{
}

//************************************************************************************
//Function:
void CConvolutionEnvCubeMapPass::initV()
{
	m_pShader = std::make_shared<CShader>("ConvolutionCubeMap_VS.glsl", "ConvolutionCubeMap_FS.glsl", "ConvolutionCubeMap_GS.glsl");
	ElayGraphics::STexture IrradianceCubeTexture;
	IrradianceCubeTexture.TextureType = ElayGraphics::STexture::ETextureType::TextureCubeMap;
	IrradianceCubeTexture.InternalFormat = GL_RGB16F;
	IrradianceCubeTexture.ExternalFormat = GL_RGB;
	IrradianceCubeTexture.DataType = GL_FLOAT;
	IrradianceCubeTexture.Width = 32;
	IrradianceCubeTexture.Height = 32;
	IrradianceCubeTexture.Type4WrapR = IrradianceCubeTexture.Type4WrapS = IrradianceCubeTexture.Type4WrapT = GL_CLAMP_TO_EDGE;
	m_IrradianceCubeMap = genTexture(IrradianceCubeTexture);
	ElayGraphics::ResourceManager::registerSharedData("IrradianceCubeTexture", m_IrradianceCubeMap);

	m_VAO = ElayGraphics::ResourceManager::getOrCreateCubeVAO();
	m_EnvCubeMap = ElayGraphics::ResourceManager::getSharedDataByName<int>("EnvCubeMap");
	m_FBO = ElayGraphics::ResourceManager::getSharedDataByName<int>("FBOWithCubeMapAttachment");
	m_VPMatrices4LookAtSixFaces = ElayGraphics::ResourceManager::getSharedDataByName<std::vector<glm::mat4>>("VPMatrices4LookAtSixFaces");

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_IrradianceCubeMap, 0);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 32, 32);
	m_pShader->activeShader();
	for (int i = 0; i < 6; ++i)
		m_pShader->setMat4UniformValue("u_VPMatrices4LookAtSixFaces[" + std::to_string(i) + "]", glm::value_ptr(m_VPMatrices4LookAtSixFaces[i]));
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubeMap);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//************************************************************************************
//Function:
void CConvolutionEnvCubeMapPass::updateV()
{
}