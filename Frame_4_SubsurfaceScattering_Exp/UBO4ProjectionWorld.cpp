#include "UBO4ProjectionWorld.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "Utils.h"
#include "ResourceManager.h"
#include "Camera.h"

//************************************************************************************
//Function:
void CUBO4ProjectionWorld::init()
{
	m_UBO = genBuffer(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW, 0);
	glm::mat4 ProjectionMatrix = CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getProjectionMatrix();
	transferData2Buffer(GL_UNIFORM_BUFFER, m_UBO, { 0 }, { sizeof(glm::mat4) }, { value_ptr(ProjectionMatrix) });
}

//************************************************************************************
//Function:
void CUBO4ProjectionWorld::update(const glm::mat4& vModelMatrix)
{
	glm::mat4 ViewMatrix = CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getViewMatrix();
	transferData2Buffer(GL_UNIFORM_BUFFER, m_UBO, { sizeof(glm::mat4),2 * sizeof(glm::mat4) }, { sizeof(glm::mat4),sizeof(glm::mat4) }, { value_ptr(ViewMatrix),value_ptr(vModelMatrix) });
}

//************************************************************************************
//Function:
void CUBO4ProjectionWorld::updateModelMatrix(const glm::mat4& vModelMatrix)
{
	transferData2Buffer(GL_UNIFORM_BUFFER, m_UBO, { 2 * sizeof(glm::mat4) }, { sizeof(glm::mat4) }, { value_ptr(vModelMatrix) });
}

//************************************************************************************
//Function:
void CUBO4ProjectionWorld::updateViewMatrix(const glm::mat4& vViewMatrix)
{
	transferData2Buffer(GL_UNIFORM_BUFFER, m_UBO, { sizeof(glm::mat4) }, { sizeof(glm::mat4) }, { value_ptr(vViewMatrix) });
}

//************************************************************************************
//Function:
void CUBO4ProjectionWorld::updateProjectionMatrix(const glm::mat4& vProjectionMatrix)
{
	transferData2Buffer(GL_UNIFORM_BUFFER, m_UBO, { 0 }, { sizeof(glm::mat4) }, { value_ptr(vProjectionMatrix) });
}