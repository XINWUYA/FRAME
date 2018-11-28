#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>
#include <vector>

class CSphereMap2CubeMapPass : public IRenderPass
{
public:
	CSphereMap2CubeMapPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CSphereMap2CubeMapPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_CubeVAO = -1;
	int m_FBO = -1;
	int m_InputSphericalTexture = -1;
	int m_EnvCubeMap = -1;
	int m_MaxMipMapLevel = -1;
	float m_MipMapWidth = 512;
	std::vector<glm::mat4> m_VPMatrices4LookAtSixFaces;
};