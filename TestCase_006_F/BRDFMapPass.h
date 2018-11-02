#pragma once
#include "RenderPass.h"

class CBRDFMapPass : public IRenderPass
{
public:
	CBRDFMapPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CBRDFMapPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_BRDFMap = -1;
	int m_FBO = -1;
};