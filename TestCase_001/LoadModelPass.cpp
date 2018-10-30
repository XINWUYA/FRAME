#include "LoadModelPass.h"
#include <GLM/gtc/type_ptr.hpp>
#include "MemoryLeakChecker.h"
#include "Interface.h"
#include "Common.h"
#include "Shader.h"
#include "GameObject.h"

CLoadModelPass::CLoadModelPass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

//************************************************************************************
//Function:
void CLoadModelPass::initV()
{
	const glm::dvec3 &CameraPos = ElayGraphics::Camera::getMainCameraPos();
	m_pShader = std::make_shared<CShader>("RenderModel_VS.glsl", "RenderModel_FS.glsl");
	m_pShader->activeShader();
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
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(ElayGraphics::ResourceManager::getGameObjectByName("Bunny")->getModelMatrix()));
	m_pShader->setTexture2DUniformValue("u_BackDepthTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("BackDepthTexture")->TextureID, 5);
	ElayGraphics::ResourceManager::getGameObjectByName("Bunny")->updateModel(*m_pShader);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}