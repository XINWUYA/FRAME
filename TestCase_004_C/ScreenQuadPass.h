#pragma once
#include "RenderPass.h"

class CScreenQuadPass : public IRenderPass
{
public:
	CScreenQuadPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CScreenQuadPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_VAO = -1;
	int m_FBOTextureArray = -1;
	int m_Layer = 0;
	int m_OldKeyRightStatus = -1;
};