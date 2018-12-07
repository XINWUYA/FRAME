#include "LightSource.h"
#include "Interface.h"
#include <random>

CLightSource::CLightSource(const std::string &vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

//************************************************************************************
//Function:
void CLightSource::initV()
{
	m_MaxLightNum = 100;
	m_LightSet.reserve(m_MaxLightNum);
	std::default_random_engine E;
	std::uniform_real_distribution<float> UPosX(-5.0f, 5.0f);
	std::uniform_real_distribution<float> UPosY(1.0, 5.0f);
	std::uniform_real_distribution<float> UPosZ(-5.0f, 5.0f);
	std::uniform_real_distribution<float> UColor(0.0f, 1.0f);
	SLight Light;
	/*Light.Position = { 0,1,0,1};
	Light.Color = { 1,1,1,1 };
	m_LightSet.push_back(Light);*/
	for (int i = 0; i < m_MaxLightNum; ++i)
	{
		Light.Position = { UPosX(E), UPosY(E), UPosZ(E),1.0 };
		Light.Color = { UColor(E),UColor(E) ,UColor(E) ,1.0 };
		m_LightSet.push_back(Light);
	}

	ElayGraphics::ResourceManager::registerSharedData("LightInfo", m_LightSet.data());
	ElayGraphics::ResourceManager::registerSharedData("LightInfoByteSize", m_LightSet.size() * sizeof(SLight));
	ElayGraphics::ResourceManager::registerSharedData("LightNum", m_LightSet.size());
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