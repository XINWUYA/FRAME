#include "ScreenQuadPass.h"
#include "Utils.h"
#include "Shader.h"
#include "Interface.h"
#include "Gameobject.h"

CScreenQuadPass::CScreenQuadPass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

//************************************************************************************
//Function:
void CScreenQuadPass::initV()
{
	m_Texture = ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("BackDepthTexture")->TextureID;
	m_pShader = std::make_shared<CShader>("ScreenQuad_VS.glsl", "ScreenQuad_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_Texture", m_Texture, 0);		//Fixed Me: 0 maybe result in conflict with model texture
}

//************************************************************************************
//Function:
void CScreenQuadPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}