#include "SphereSetPass.h"
#include <GLM/glm.hpp>
#include "SpherePass.h"
#include "Interface.h"
#include "GameObject.h"
#include "SphereSet.h"

CSphereSetPass::CSphereSetPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CSphereSetPass::~CSphereSetPass()
{
}

//************************************************************************************
//Function:
void CSphereSetPass::initV()
{
	m_RowNum = ElayGraphics::ResourceManager::getSharedDataByName<int>("SphereRowNum");
	m_ColNum = ElayGraphics::ResourceManager::getSharedDataByName<int>("SphereColNum");
	m_SpherePassSet.resize(m_RowNum);
	for (int Row = 0; Row < m_RowNum; ++Row)
	{
		m_SpherePassSet[Row].resize(m_ColNum);
		for (int Col = 0; Col < m_ColNum; ++Col)
		{
			CSpherePass& SpherePass = m_SpherePassSet[Row][Col];
			SpherePass.setMetalness(float(Row) / m_RowNum);
			SpherePass.setRoughness(glm::clamp(float(Col) / m_ColNum, 0.1f, 1.0f));
			SpherePass.setModelMatrix(std::dynamic_pointer_cast<CSphereSet>(ElayGraphics::ResourceManager::getGameObjectByName("SphereSet"))->getModelMatrixBySphereIndex(Row, Col));
			SpherePass.initV();
		}
	}
}

//************************************************************************************
//Function:
void CSphereSetPass::updateV()
{
	for (int Row = 0; Row < m_RowNum; ++Row)
	{
		for (int Col = 0; Col < m_ColNum; ++Col)
		{
			CSpherePass& SpherePass = m_SpherePassSet[Row][Col];
			SpherePass.updateV();
		}
	}
}