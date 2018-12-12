#include "LightSource.h"
#include "Interface.h"

CLightSource::CLightSource(const std::string &vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

//************************************************************************************
//Function:
void CLightSource::initV()
{
	//m_LightPos = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightPosition");
}

//************************************************************************************
//Function:
void CLightSource::updateV()
{
	bool LightPosChanged = false;
	m_LightPos = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("Position", m_LightPos, LightPosChanged);
	if (LightPosChanged)
		setPosition(m_LightPos);
}