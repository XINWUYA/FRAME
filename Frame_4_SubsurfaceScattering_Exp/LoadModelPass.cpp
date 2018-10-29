#include "LoadModelPass.h"
#include "ResourceManager.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

CLoadModelPass::CLoadModelPass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

//************************************************************************************
//Function:
void CLoadModelPass::initV()
{
	CResourceManager::getOrCreateInstance()->getOrCreateModel("../../Model/Bunny/bunny.obj");
	const glm::dvec3 &CameraPos  = CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraPos();
	m_pShader = std::make_shared<CShader>("RenderModel_VS.glsl", "RenderModel_FS.glsl");
	m_pShader->setFloatUniformValue("u_ViewPos", static_cast<GLfloat>(CameraPos.x), static_cast<GLfloat>(CameraPos.y), static_cast<GLfloat>(CameraPos.z));
}

//************************************************************************************
//Function:
void CLoadModelPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	m_pShader->activeShader();
	m_pShader->setTexture2DUniformValue("u_BackDepthTexture", CResourceManager::getOrCreateInstance()->getTexture2DByName("BackDepthTexture")->TextureID, 5);
	CResourceManager::getOrCreateInstance()->getOrCreateModel("../../Model/Bunny/bunny.obj")->update(*m_pShader);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}