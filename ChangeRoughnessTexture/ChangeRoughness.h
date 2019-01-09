#pragma once
#include "RenderPass.h"

class CChangeRoughnessPass : public IRenderPass
{
public:
	CChangeRoughnessPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CChangeRoughnessPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_Texture = -1;
};