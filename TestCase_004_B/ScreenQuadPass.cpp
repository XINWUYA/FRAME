#include "ScreenQuadPass.h"
#include <iostream>
#include <string>
#include "Interface.h"
#include "Shader.h"
#include "Common.h"
#include "Utils.h"
#include <GLFW/glfw3.h>

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
	m_pShader = std::make_shared<CShader>("ScreenQuad_VS.glsl", "ScreenQuad_FS.glsl");
	m_FBOTextureArray = ElayGraphics::ResourceManager::getSharedDataByName<int>("FBOTextureArray");

	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_TextureArray", m_FBOTextureArray, 1, GL_TEXTURE_2D_ARRAY);
}

//************************************************************************************
//Function:
void CScreenQuadPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShader->activeShader();
	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_RIGHT) == GLFW_PRESS && m_OldKeyRightStatus != GLFW_PRESS)
	{
		m_OldKeyRightStatus = GLFW_PRESS;
		m_Layer = (m_Layer + 1) % 5;
		m_pShader->setIntUniformValue("u_Layer", m_Layer);
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_RIGHT) == GLFW_RELEASE)
		m_OldKeyRightStatus = GLFW_RELEASE;
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}