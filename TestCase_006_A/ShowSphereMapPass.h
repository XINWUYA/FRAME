#pragma once
#include "RenderPass.h"

class CShowSphereMapPass : public IRenderPass
{
public:
	CShowSphereMapPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CShowSphereMapPass();

	virtual void initV();
	virtual void updateV();

private:
	int m_HDRTexture = 0;
	int m_CubeVAO = 0;
};