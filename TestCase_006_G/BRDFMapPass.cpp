#include "BRDFMapPass.h"
#include "Common.h"
#include "Interface.h"
#include "Utils.h"
#include "Shader.h"

CBRDFMapPass::CBRDFMapPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CBRDFMapPass::~CBRDFMapPass()
{
}

//************************************************************************************
//Function:
void CBRDFMapPass::initV()
{
	m_pShader = std::make_shared<CShader>("BRDFMap_VS.glsl", "BRDFMap_FS.glsl");

	ElayGraphics::STexture BRDFTexture;
	BRDFTexture.InternalFormat = GL_RG16F;
	BRDFTexture.ExternalFormat = GL_RG;
	BRDFTexture.DataType = GL_FLOAT;
	BRDFTexture.Type4WrapT = BRDFTexture.Type4WrapS = GL_CLAMP_TO_EDGE;
	BRDFTexture.Width = 512;
	BRDFTexture.Height = 512;
	m_BRDFMap = genTexture(BRDFTexture);
	ElayGraphics::ResourceManager::registerSharedData("BRDFMap", m_BRDFMap);

	m_FBO = genFBO({ BRDFTexture });

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BRDFMap, 0);
	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//************************************************************************************
//Function:
void CBRDFMapPass::updateV()
{
}