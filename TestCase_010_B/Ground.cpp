#include "Ground.h"
#include "Interface.h"

CGround::CGround(const std::string& vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

void CGround::initV()
{
	setRotationX(90.0f);
	scale(glm::vec3(10, 10, 1));
}

void CGround::updateV()
{
}