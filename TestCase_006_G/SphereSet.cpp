#include "SphereSet.h"
#include "Sphere.h"
#include "Interface.h"

CSphereSet::CSphereSet(const std::string &vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

CSphereSet::~CSphereSet()
{
}

//************************************************************************************
//Function:
void CSphereSet::initV()
{
	ElayGraphics::ResourceManager::registerSharedData("SphereRowNum", m_RowNum);
	ElayGraphics::ResourceManager::registerSharedData("SphereColNum", m_ColNum);
	m_SphereSet.resize(m_RowNum);

	for (int Row = 0; Row < m_RowNum; ++Row)
	{
		m_SphereSet[Row].resize(m_ColNum);
		for (int Col = 0; Col < m_ColNum; ++Col)
			m_SphereSet[Row][Col].translate(glm::vec3((Col - m_ColNum / 2) * m_SpacingBetweenSpheres, (Row - m_RowNum / 2) * m_SpacingBetweenSpheres, -2.0f));
	}
}

//************************************************************************************
//Function:
void CSphereSet::updateV()
{
}

//************************************************************************************
//Function:
const glm::mat4& CSphereSet::getModelMatrixBySphereIndex(int vRowIndex, int vColIndex) const 
{
	return m_SphereSet[vRowIndex][vColIndex].getModelMatrix();
}