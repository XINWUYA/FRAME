#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>

class CConvolutionEnvCubeMapPass : public IRenderPass
{
public:
	CConvolutionEnvCubeMapPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CConvolutionEnvCubeMapPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_VAO = -1;
	int m_FBO = -1;
	int m_IrradianceCubeMap = -1;
	int m_EnvCubeMap = -1;
	std::vector<glm::mat4> m_VPMatrices4LookAtSixFaces;
};