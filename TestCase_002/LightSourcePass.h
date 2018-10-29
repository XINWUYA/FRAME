#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>

class CLightSourcePass : public IRenderPass
{
public:
	CLightSourcePass(const std::string& vPassName, int vExcutionOrder);

	virtual void initV() override;
	virtual void updateV() override;
};