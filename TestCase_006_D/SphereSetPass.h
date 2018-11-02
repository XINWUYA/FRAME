#pragma once
#include "RenderPass.h"
#include <vector>

class CSpherePass;

class CSphereSetPass : public IRenderPass
{
public:
	CSphereSetPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CSphereSetPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_RowNum = 0;
	int m_ColNum = 0;
	std::vector<std::vector<CSpherePass>> m_SpherePassSet;
};