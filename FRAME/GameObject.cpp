#include "GameObject.h"
#include <GLM/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Model.h"

IGameObject::IGameObject()
{
}

IGameObject::IGameObject(const std::string &vGameObjectName, int vExecutionOrder) : m_Name(vGameObjectName), m_ExecutionOrder(vExecutionOrder)
{
}

IGameObject::~IGameObject()
{
}

//************************************************************************************
//Function:
bool IGameObject::operator<(const IGameObject& vOtherPass) const
{
	return m_ExecutionOrder < vOtherPass.getExecutationOrder();
}

//************************************************************************************
//Function:
const glm::vec3& IGameObject::getPosition() const
{
	return m_Position;
}

//************************************************************************************
//Function:
const glm::vec3& IGameObject::getRotation() const
{
	return m_RotationAngle;
}

//************************************************************************************
//Function:
const glm::vec3& IGameObject::getScale() const 
{
	return m_Scale;
}

//************************************************************************************
//Function:
void IGameObject::setPosition(const glm::vec3& vPosition)
{
	m_ModelMatrix = glm::translate(m_ModelMatrix, vPosition - m_Position);
	m_Position = vPosition;
}

//************************************************************************************
//Function:
void IGameObject::setRotation(const glm::vec3& vRotationAngle)
{
	if (vRotationAngle[0])
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationAngle[0] - m_RotationAngle[0]), glm::vec3(1, 0, 0));
	if (vRotationAngle[1])
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationAngle[1] - m_RotationAngle[1]), glm::vec3(0, 1, 0));
	if (vRotationAngle[2])
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationAngle[2] - m_RotationAngle[2]), glm::vec3(0, 0, 1));
	m_RotationAngle = vRotationAngle;
}

//************************************************************************************
//Function:
void IGameObject::setScale(const glm::vec3& vScale)
{
	m_ModelMatrix = glm::scale(m_ModelMatrix, vScale / m_Scale);
	m_Scale = vScale;
}

//************************************************************************************
//Function:
void IGameObject::translate(const glm::vec3& vPositionOffset)
{
	m_ModelMatrix = glm::translate(m_ModelMatrix, vPositionOffset);
	m_Position += vPositionOffset;
}

//************************************************************************************
//Function:
void IGameObject::rotate(const glm::vec3& vRotationOffset)
{
	if (vRotationOffset[0])
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationOffset[0]), glm::vec3(1, 0, 0));
	if (vRotationOffset[1])
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationOffset[1]), glm::vec3(0, 1, 0));
	if (vRotationOffset[2])
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationOffset[2]), glm::vec3(0, 0, 1));
	m_RotationAngle += vRotationOffset;
}

//************************************************************************************
//Function:
void IGameObject::scale(const glm::vec3& vScaleOffset)
{
	m_ModelMatrix = glm::scale(m_ModelMatrix, vScaleOffset);
	m_Scale *= vScaleOffset;
}

//************************************************************************************
//Function:
const glm::mat4& IGameObject::getModelMatrix() const
{
	return m_ModelMatrix;
}

//************************************************************************************
//Function:
int IGameObject::getVAO() const 
{
	return m_VAO;
}

//************************************************************************************
//Function:
void IGameObject::updateModel(const CShader& vShader) const
{
	m_pModel->update(vShader);
}