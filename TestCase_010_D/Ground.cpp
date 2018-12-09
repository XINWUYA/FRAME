#include "Ground.h"
#include "Interface.h"

CGround::CGround(const std::string& vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

void CGround::initV()
{
	rotate(glm::vec3(-90, 0, 0));
	scale(glm::vec3(10, 10, 1));
}

void CGround::updateV()
{
	bool RotationChanged = false;
	glm::vec3 RotationFromGUI = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("Rotation", m_Rotation, RotationChanged);
	if (RotationChanged)
	{
		m_Rotation = RotationFromGUI;
		setRotation(m_Rotation);
	}
}