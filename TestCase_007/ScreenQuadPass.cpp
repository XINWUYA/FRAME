#include "ScreenQuadPass.h"
#include "Utils.h"
#include "Common.h"
#include "Shader.h"

CScreenQuadPass::CScreenQuadPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CScreenQuadPass::~CScreenQuadPass()
{
}

//************************************************************************************
//Function:
void CScreenQuadPass::initV()
{
	ElayGraphics::STexture Texture2D;
	Texture2D.BorderColor = { 1,0,0,1 };
	Texture2D.Type4WrapS = Texture2D.Type4WrapT = GL_CLAMP_TO_BORDER;
	m_Texture = loadTextureFromFile("../Textures/WZRY/0.jpg", Texture2D);
	m_pShader = std::make_shared<CShader>("ScreenQuad_VS.glsl", "ScreenQuad_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_Texture", m_Texture, 0);
}

//************************************************************************************
//Function:
void CScreenQuadPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	drawQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}