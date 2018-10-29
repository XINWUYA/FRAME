#pragma once
#include <GL/glew.h>
#include "RenderPass.h"

class CShader;

class CScreenQuadPass:public IRenderPass
{
public:
	CScreenQuadPass(const std::string& vPassName, int vExcutionOrder);

	virtual void initV() override;
	virtual void updateV() override;

private:
	GLint m_VAO;
	GLint m_Texture;
};