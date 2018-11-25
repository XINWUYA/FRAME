#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>
#include <vector>

class CSixFaceMap2CubeMapPass : public IRenderPass
{
public:
	CSixFaceMap2CubeMapPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CSixFaceMap2CubeMapPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_CubeVAO = -1;
	int m_FBO = -1;
	int m_HDRTexture = -1;
	int m_EnvCubeMap = -1;
	int m_FaceSize = 512;
	std::vector<glm::mat4> m_VPMatrices4LookAtSixFaces;
};