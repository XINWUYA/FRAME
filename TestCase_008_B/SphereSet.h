#pragma once
#include "GameObject.h"
#include <vector>
#include <memory>
#include <GLM/gtc/matrix_transform.hpp>

class CSphere;

class CSphereSet : public IGameObject
{
public:
	CSphereSet(const std::string &vGameObjectName, int vExecutionOrder);
	~CSphereSet();

	virtual void initV() override;
	virtual void updateV() override;

	const glm::mat4& getModelMatrixBySphereIndex(int vRowIndex, int vColIndex) const;

private:
	std::vector<std::vector<CSphere>> m_SphereSet;
	int	  m_RowNum = 7;
	int   m_ColNum = 7;
	float m_SpacingBetweenSpheres = 2.5f;
};