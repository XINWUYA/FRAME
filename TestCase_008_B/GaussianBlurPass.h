#pragma once
#include "RenderPass.h"
#include <vector>

class CGaussianBlurPass : public IRenderPass
{
public:
	CGaussianBlurPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CGaussianBlurPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_PingPongFBO{ 0,0 };
	std::vector<int> m_PingPongTexture{ 0,0 };
	int m_InputTexture = 0;
	int m_MaxMipLevel = 8;
};