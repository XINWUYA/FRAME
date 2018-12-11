#pragma once
#include "RenderPass.h"
#include <memory>
#include <GLM/glm.hpp>

class CLightSource;

class CLightSourcePass : public IRenderPass
{
public:
	CLightSourcePass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CLightSourcePass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_LightSourcePosVAO = -1;
	int m_LightSourcePosVBO = -1;
	glm::vec3 m_LightSourcePos;
	std::shared_ptr<CLightSource> m_pLightSource;
};