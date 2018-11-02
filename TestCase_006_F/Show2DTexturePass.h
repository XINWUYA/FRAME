#pragma once
#include "RenderPass.h"

class CShow2DTexturePass : public IRenderPass
{
public:
	CShow2DTexturePass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CShow2DTexturePass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_Texture = -1;
};