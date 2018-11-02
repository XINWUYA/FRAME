#pragma once
#include "RenderPass.h"
#include <memory>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

class CSphereSet;

class CSpherePass : public IRenderPass
{
public:
	CSpherePass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CSpherePass();

	virtual void initV() override;
	virtual void updateV() override;

	//void setRoughness(float vRoughness) { m_Roughness = vRoughness; }
	//void setMetalness(float vMetalness) { m_Metalness = vMetalness; }
	//void setModelMatrix(const glm::mat4& vModelMatrix) { m_ModelMatrix = vModelMatrix; }

private:
	int	  m_IrradianceCubeMap = 0;
	int   m_RowNum = 0;
	int   m_ColNum = 0;
	float m_Metalness = 0.0f;
	float m_Roughness = 0.0f;
	glm::mat4 m_ModelMatrix;
	std::shared_ptr<CSphereSet> m_SphereSet;
};