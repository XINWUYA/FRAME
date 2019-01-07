#include "LightSource.h"
#include "Interface.h"
#include <random>

const float MODEL_BORDER = 5.0f;

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
	m_LightMovingDirSet.reserve(m_MaxLightNum);

	SLight Light;
	for (int i = 0; i < m_MaxLightNum; ++i)
	{
		float x = __generateRandomFloat(-5.0f, 5.0f);
		float y = __generateRandomFloat(-5.0f, 5.0f);
		float z = __generateRandomFloat(-5.0f, 5.0f);
		Light.Position = { x, y, z, 1.0 };
		Light.Color = { __generateRandomFloat(0.0f, 1.0f), __generateRandomFloat(0.0f, 1.0f), __generateRandomFloat(0.0f, 1.0f) ,1.0 };
		m_LightSet.push_back(Light);

		glm::vec3 MovingDir = glm::normalize(glm::vec3(__generateRandomFloat(-1.0f, 1.0f), __generateRandomFloat(-1.0f, 1.0f), __generateRandomFloat(-1.0f, 1.0f)));
		m_LightMovingDirSet.push_back(MovingDir);
	}

	ElayGraphics::ResourceManager::registerSharedData("LightInfo", m_LightSet.data());
	ElayGraphics::ResourceManager::registerSharedData("LightInfoByteSize", m_LightSet.size() * sizeof(SLight));
	ElayGraphics::ResourceManager::registerSharedData("LightNum", m_LightSet.size());
}

//************************************************************************************
//Function:
void CLightSource::updateV()
{
	for (int i = 0; i < m_MaxLightNum; ++i)
	{
		glm::vec3 NextPos = glm::vec3(m_LightSet[i].Position) + m_StepMovingLength * m_LightMovingDirSet[i];
		while (std::abs(NextPos.x) >= MODEL_BORDER || std::abs(NextPos.y) >= MODEL_BORDER || std::abs(NextPos.z) >= MODEL_BORDER)
		{
			m_LightMovingDirSet[i] = glm::normalize(glm::vec3(__generateRandomFloat(-1.0f, 1.0f), __generateRandomFloat(-1.0f, 1.0f), __generateRandomFloat(-1.0f, 1.0f)));
			NextPos = glm::vec3(m_LightSet[i].Position) + m_StepMovingLength * m_LightMovingDirSet[i];
		}
		m_LightSet[i].Position = glm::vec4(NextPos, 1.0);
	}

	bool LightPosChanged = false;
	m_LightPos = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("Position", m_LightPos, LightPosChanged);
	if (LightPosChanged)
		setPosition(m_LightPos);
}

//************************************************************************************
//Function:
float CLightSource::__generateRandomFloat(float vMinValue, float vMaxValue)
{
	std::uniform_real_distribution<float> RandomFloat(vMinValue, vMaxValue);
	return RandomFloat(m_RandomEngine);
}
