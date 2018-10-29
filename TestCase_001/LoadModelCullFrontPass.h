#pragma once
#include <GL/glew.h>
#include "RenderPass.h"

class CShader;

class CLoadModelCullFrontPass:public IRenderPass
{
public:
	CLoadModelCullFrontPass(const std::string& vPassName, int vExcutionOrder);

	virtual void initV() override;
	virtual void updateV() override;

private:
	GLint m_FBO;
};