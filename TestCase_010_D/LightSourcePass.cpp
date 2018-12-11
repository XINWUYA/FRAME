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
	m_pLightSource = std::dynamic_pointer_cast<CLightSource>(ElayGraphics::ResourceManager::getGameObjectByName("LightSource"));
	m_LightSourcePos = m_pLightSource->getLightPos();
	m_LightSourcePosVAO = createVAO(&m_LightSourcePos, sizeof(m_LightSourcePos), { 3 }, nullptr, 0, &m_LightSourcePosVBO);
}

void CLightSourcePass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_PROGRAM_POINT_SIZE);

	const glm::vec3& LightSourcePos = m_pLightSource->getLightPos();
	if (LightSourcePos != m_LightSourcePos)
	{
		m_LightSourcePos = LightSourcePos;
		transferData2Buffer(GL_ARRAY_BUFFER, m_LightSourcePosVBO, { 0 }, { sizeof(m_LightSourcePos) }, { &m_LightSourcePos });
	}

	m_pShader->activeShader();
	glBindVertexArray(m_LightSourcePosVAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);

	glDisable(GL_PROGRAM_POINT_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}