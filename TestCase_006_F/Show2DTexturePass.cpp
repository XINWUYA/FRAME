#include "Show2DTexturePass.h"
#include "Shader.h"
#include "Utils.h"
#include "Interface.h"

CShow2DTexturePass::CShow2DTexturePass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CShow2DTexturePass::~CShow2DTexturePass()
{
}

//************************************************************************************
//Function:
void CShow2DTexturePass::initV()
{
	m_pShader = std::make_shared<CShader>("Show2DTexture_VS.glsl", "Show2DTexture_FS.glsl");
	m_Texture = ElayGraphics::ResourceManager::getSharedDataByName<int>("BRDFMap");
}

//************************************************************************************
//Function:
void CShow2DTexturePass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShader->activeShader();
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	drawQuad();
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}