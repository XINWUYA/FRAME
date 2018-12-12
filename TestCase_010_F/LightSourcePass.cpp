#include "LightSourcePass.h"
#include "LightSource.h"
#include "Utils.h"
#include "Interface.h"
#include "GameObject.h"
#include "Shader.h"
#include <GLM/gtc/type_ptr.hpp>

CLightSourcePass::CLightSourcePass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CLightSourcePass::~CLightSourcePass()
{
}

void CLightSourcePass::initV()
{
	m_pShader = std::make_shared<CShader>("LightSource_VS.glsl", "LightSource_FS.glsl");
	const glm::vec3& LightSourcePos = std::dynamic_pointer_cast<CLightSource>(ElayGraphics::ResourceManager::getGameObjectByName("LightSource"))->getLightPos();
	m_LightSourcePosVAO = createVAO(&LightSourcePos, sizeof(LightSourcePos), { 3 });
}

void CLightSourcePass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glBindVertexArray(m_LightSourcePosVAO);
	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(ElayGraphics::ResourceManager::getGameObjectByName("LightSource")->getModelMatrix()));
	glDrawArrays(GL_POINTS, 0, 1);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}