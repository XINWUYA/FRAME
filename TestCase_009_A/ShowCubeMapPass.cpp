#include "ShowCubeMapPass.h"
#include "Shader.h"
#include "Interface.h"

CShowCubeMapPass::CShowCubeMapPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CShowCubeMapPass::~CShowCubeMapPass()
{
}

//************************************************************************************
//Function:
void CShowCubeMapPass::initV()
{
	m_pShader = std::make_shared<CShader>("ShowCubeMap_VS.glsl", "ShowCubeMap_FS.glsl");
	m_CubeVAO = ElayGraphics::ResourceManager::getOrCreateCubeVAO();
	m_EnvCubeMap = ElayGraphics::ResourceManager::getSharedDataByName<int>("EnvCubeMap");
}

//************************************************************************************
//Function:
void CShowCubeMapPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShader->activeShader();
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubeMap);
	glBindVertexArray(m_CubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}