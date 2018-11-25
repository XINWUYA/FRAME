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
	int m_InputTexture = -1;
	int m_FBO[2] = { -1,-1 };
	int m_FBOTexture[2] = { -1,-1 };
	float m_GaussianSigma = 60.0;
	std::vector<float> m_GaussianWeightSet;

	void __calculateGaussianWeightSet();
};