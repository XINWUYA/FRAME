#pragma once
#include <GL/glew.h>
#include "RenderPass.h"

class CShader;

class CLoadModelPass :public IRenderPass
{
public:
	CLoadModelPass(const std::string& vPassName, int vExcutionOrder);

	virtual void initV() override;
	virtual void updateV() override;

private:

};