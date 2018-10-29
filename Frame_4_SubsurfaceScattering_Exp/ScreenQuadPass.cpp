#include "ScreenQuadPass.h"
#include "Utils.h"
#include "ResourceManager.h"
#include "Shader.h"

CScreenQuadPass::CScreenQuadPass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

//************************************************************************************
//Function:
void CScreenQuadPass::initV()
{
	m_VAO = createVAO4ScreenQuad();
	//m_Texture = loadTextureFromFile("container.jpg");
	m_Texture = CResourceManager::getOrCreateInstance()->getTexture2DByName("BackDepthTexture")->TextureID;
	m_pShader = std::make_shared<CShader>("ScreenQuad_VS.glsl", "ScreenQuad_FS.glsl");
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
	m_pShader->setTexture2DUniformValue("u_Texture", m_Texture, 0);		//Fixed Me: 0 maybe result in conflict with model texture
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}