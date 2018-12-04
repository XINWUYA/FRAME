#pragma once
#include "GameObject.h"
#include <vector>

class CLightSource : public IGameObject
{
public:
	CLightSource(const std::string &vGameObjectName, int vExecutionOrder);

	virtual void initV() override;
	virtual void updateV() override;

	const glm::vec3& getLightPos() const { return m_LightPos; }

private:
	glm::vec3 m_LightPos;
};