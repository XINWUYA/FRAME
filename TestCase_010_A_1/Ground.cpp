#include "Ground.h"
#include "Interface.h"

CGround::CGround(const std::string& vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

void CGround::initV()
{
	m_Rotation = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("Ground Rotation");
	//rotate(m_Rotation);
	setRotationX(m_Rotation[0]);
	setRotationY(m_Rotation[1]);
	setRotationZ(m_Rotation[2]);
	setScale(glm::vec3(10, 10, 1));
}

void CGround::updateV()
{
	bool RotationChanged = false;
	glm::vec3 RotationFromGUI = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("Ground Rotation", m_Rotation, RotationChanged);
	if (RotationChanged)
	{
		m_Rotation = RotationFromGUI;
		setRotationX(m_Rotation[0]);
		setRotationY(m_Rotation[1]);
		setRotationZ(m_Rotation[2]);
	}
}