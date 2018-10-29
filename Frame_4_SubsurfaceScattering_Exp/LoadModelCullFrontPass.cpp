#include "LoadModelCullFrontPass.h"
#include "ResourceManager.h"
#include "Utils.h"
#include "common.h"
#include "Shader.h"
#include "Model.h"

CLoadModelCullFrontPass::CLoadModelCullFrontPass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

//************************************************************************************
//Function:
void CLoadModelCullFrontPass::initV()
{
	std::shared_ptr<ElayGraphics::STexture2D> pTexture2D = std::make_shared<ElayGraphics::STexture2D>();
	pTexture2D->InternalFormat = GL_DEPTH_COMPONENT;
	pTexture2D->ExternalFormat = GL_DEPTH_COMPONENT;
	pTexture2D->DataType = GL_FLOAT;
	pTexture2D->TextureType = ElayGraphics::STexture2D::DepthTexture;
	pTexture2D->TextureName = "BackDepthTexture";
	genTexture2D(*pTexture2D);
	CResourceManager::getOrCreateInstance()->registerTexture2D(pTexture2D);
	m_FBO = genFBO({ *pTexture2D });
	m_pShader = std::make_shared<CShader>("LoadModel_VS.glsl", "LoadModel_FS.glsl");
}

//************************************************************************************
//Function:
void CLoadModelCullFrontPass::updateV()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	m_pShader->activeShader();
	CResourceManager::getOrCreateInstance()->getOrCreateModel("../../Model/Bunny/bunny.obj")->update(*m_pShader);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}