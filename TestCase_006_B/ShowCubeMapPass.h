#pragma once
#include "RenderPass.h"

class CShowCubeMapPass : public IRenderPass
{
public:
	CShowCubeMapPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CShowCubeMapPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_CubeVAO = -1;
	int m_EnvCubeMap = -1;
};