#include "LightSourcePass.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Interface.h"
#include "GameObject.h"

CLightSourcePass::CLightSourcePass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

//************************************************************************************
//Function:
void CLightSourcePass::initV()
{
	m_pShader = std::make_shared<CShader>("LightSource_VS.glsl", "LightSource_FS.glsl");
}

//************************************************************************************
//Function:
void CLightSourcePass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(ElayGraphics::ResourceManager::getGameObjectByName("LightSource")->getModelMatrix()));
	glBindVertexArray(ElayGraphics::ResourceManager::getGameObjectByName("LightSource")->getVAO());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}