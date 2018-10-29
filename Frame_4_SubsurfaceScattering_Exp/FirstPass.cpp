#include "FirstPass.h"
#include <SOIL/SOIL.h>
#include "Utils.h"
#include "Shader.h"
#include "common.h"
#include "ResourceManager.h"
#include "Model.h"

CFirstPass::CFirstPass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

//************************************************************************************
//Function:
void CFirstPass::initV()
{
	std::shared_ptr<ElayGraphics::STexture2D> pTexture2D = std::make_shared<ElayGraphics::STexture2D>();
	genTexture2D(*pTexture2D);
	CResourceManager::getOrCreateInstance()->registerTexture2D(pTexture2D);
	m_FBO = genFBO({ *pTexture2D });
	m_pShader = std::make_shared<CShader>("PointsVS.glsl", "PointsFS.glsl");
}

//************************************************************************************
//Function:
void CFirstPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	CResourceManager::getOrCreateInstance()->getOrCreateModel("../../Model/Bunny/bunny.obj")->update(*m_pShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}