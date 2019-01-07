#pragma once
#include "GameObject.h"
#include <vector>
#include <random>

struct SLight
{
	glm::vec4 Position;
	glm::vec4 Color;
};

class CLightSource : public IGameObject
{
public:
	CLightSource(const std::string &vGameObjectName, int vExecutionOrder);

	virtual void initV() override;
	virtual void updateV() override;

	const glm::vec3& getLightPos() const { return m_LightPos; }

private:
	float __generateRandomFloat(float vMinValue, float vMaxValue);

	int m_MaxLightNum;
	float m_StepMovingLength = 0.01;
	glm::vec3 m_LightPos;
	std::vector<SLight> m_LightSet;
	std::vector<glm::vec3> m_LightMovingDirSet;
	std::default_random_engine m_RandomEngine;
};