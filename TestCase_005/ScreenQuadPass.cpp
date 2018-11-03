#include "ScreenQuadPass.h"
#include <GL/glew.h>
#include "Interface.h"
#include "Shader.h"

CScreenQuadPass::CScreenQuadPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CScreenQuadPass::~CScreenQuadPass()
{
}

//************************************************************************************
//Function:
void CScreenQuadPass::initV()
{
	m_VAO = ElayGraphics::ResourceManager::getOrCreateScreenQuadVAO();
	m_HDRTexture = ElayGraphics::ResourceManager::getSharedDataByName<int>("HDRTexture");
	m_pShader = std::make_shared<CShader>("ScreenQuad_VS.glsl", "ScreenQuad_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_HDRTexture", m_HDRTexture, 2);
}

//************************************************************************************
//Function:
void CScreenQuadPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShader->activeShader();
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_HDRTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}