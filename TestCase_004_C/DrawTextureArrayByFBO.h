#pragma once
#include "RenderPass.h"

class CDrawTextureArrayByFBO : public IRenderPass
{
public:
	CDrawTextureArrayByFBO(const std::string& vPassName, int vExecutionOrder);
	virtual ~CDrawTextureArrayByFBO();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_VAO = -1;
	int m_InputTextureArray = -1;
	int m_FBO[2] = { -1,-1 };
	int m_FBOTextureArray[2] = { -1,-1 };
};