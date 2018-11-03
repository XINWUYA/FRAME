#include "LightSourcePass.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Interface.h"
#include "GameObject.h"
#include "Utils.h"

CLightSourcePass::CLightSourcePass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

CLightSourcePass::~CLightSourcePass()
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
	float Intensity = ElayGraphics::ResourceManager::getSharedDataByName<float>("Intensity");
	if (Intensity != m_Intensity)
	{
		m_Intensity = Intensity;
		m_pShader->setFloatUniformValue("u_Intensity", m_Intensity);
	}
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}