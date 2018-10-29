#pragma once
#include <GL/glew.h>
#include "RenderPass.h"

class CShader;

class CFirstPass : public IRenderPass
{
public:
	CFirstPass(const std::string& vPassName, int vExcutionOrder);

	virtual void initV() override;
	virtual void updateV() override;

private:
	GLint m_FBO;
};