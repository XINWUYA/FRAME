#include "ShowSphereMapPass.h"
#include "Interface.h"
#include "Utils.h"
#include "Shader.h"

CShowSphereMapPass::CShowSphereMapPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CShowSphereMapPass::~CShowSphereMapPass()
{
}

//************************************************************************************
//Function:
void CShowSphereMapPass::initV()
{
	m_HDRTexture = loadTextureFromFile("../Textures/hdr/newport_loft.hdr");
	m_CubeVAO = ElayGraphics::ResourceManager::getOrCreateCubeVAO();
	m_pShader = std::make_shared<CShader>("ShowSphereMap_VS.glsl", "ShowSphereMap_FS.glsl");
	m_pShader->activeShader();
	glBindTexture(GL_TEXTURE_2D, m_HDRTexture);
}

//************************************************************************************
//Function:
void CShowSphereMapPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_pShader->activeShader();
	glBindVertexArray(m_CubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}