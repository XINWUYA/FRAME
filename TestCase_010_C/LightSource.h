#pragma once
#include "GameObject.h"
#include <vector>
#include <GLM/glm.hpp>

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
	int m_MaxLightNum;
	glm::vec3 m_LightPos;
	std::vector<SLight> m_LightSet;
};