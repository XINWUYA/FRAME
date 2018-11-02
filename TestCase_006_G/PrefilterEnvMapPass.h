#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/gtc/matrix_transform.hpp>

class CPrefilterEnvMapPass : public IRenderPass
{
public:
	CPrefilterEnvMapPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CPrefilterEnvMapPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_PrefilterEnvMap = -1;
	int m_EnvCubeMap = -1;
	int m_FBO = -1;
	std::vector<glm::mat4> m_VPMatrices4LookAtSixFaces;
};