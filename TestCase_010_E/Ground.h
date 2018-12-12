#pragma once
#include "GameObject.h"
#include <GLM/glm.hpp>

class CGround : public IGameObject
{
public:
	CGround(const std::string& vGameObjectName, int vExecutionOrder);

	virtual void initV() override;
	virtual void updateV() override;

private:
	glm::vec3 m_Rotation;
};