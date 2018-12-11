#include "LightSourcePass.h"
#include "LightSource.h"
#include "Utils.h"
#include "Interface.h"
#include "GameObject.h"
#include "Shader.h"

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
	glEnable(GL_PROGRAM_POINT_SIZE);

	m_pShader->activeShader();
	glBindVertexArray(m_LightSourcePosVAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);

	glDisable(GL_PROGRAM_POINT_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}