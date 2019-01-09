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
	m_LightPos = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("Position");

	m_MaxLightNum = 100;
	m_LightSet.reserve(m_MaxLightNum);
	std::default_random_engine E;
	std::uniform_real_distribution<float> UPosX(-5.0f, 5.0f);
	std::uniform_real_distribution<float> UPosY(-5.0, 5.0f);
	std::uniform_real_distribution<float> UPosZ(-5.0f, 5.0f);
	std::uniform_real_distribution<float> UColor(0.0f, 1.0f);
	SLight Light;
	for (int i = 0; i < m_MaxLightNum; ++i)
	{
		float x = UPosX(E);
		float y = UPosY(E);
		float z = UPosZ(E);

		Light.Position = { x, y, z, 1.0 };
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