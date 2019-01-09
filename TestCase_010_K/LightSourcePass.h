#pragma once
#include "RenderPass.h"

class CLightSourcePass : public IRenderPass
{
public:
	CLightSourcePass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CLightSourcePass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_LightSourcePosVAO = -1;
};