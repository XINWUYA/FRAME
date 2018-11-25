#include "GaussianBlurPass.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"

CGaussianBlurPass::CGaussianBlurPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CGaussianBlurPass::~CGaussianBlurPass()
{
}

//************************************************************************************
//Function:
void CGaussianBlurPass::initV()
{
	m_pShader = std::make_shared<CShader>("GaussianBlur_VS.glsl", "GaussianBlur_FS.glsl");
	int Width = 0, Height = 0, Channels = 0;
	m_InputTexture = loadTextureFromFile("../Textures/hdr/newport_loft.hdr");

	ElayGraphics::STexture FBOTexture2D;
	for (int i = 0; i < 2; ++i)
	{
		m_FBOTexture[i] = genTexture(FBOTexture2D);
		m_FBO[i] = genFBO({ FBOTexture2D });
	}
	ElayGraphics::ResourceManager::registerSharedData("BluredTexture", m_FBOTexture[0]);

	__calculateGaussianWeightSet();
	int UBO4GaussianWeightSet = genBuffer(GL_SHADER_STORAGE_BUFFER, m_GaussianWeightSet.size() * sizeof(float), m_GaussianWeightSet.data(), GL_STATIC_DRAW, 1);

	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_GaussianWeightNum", m_GaussianWeightSet.size());
}

//************************************************************************************
//Function:
void CGaussianBlurPass::updateV()
{
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glViewport(0, 0, m_Width, m_Height);

	bool IsHorizontal = true;
	bool IsFirst = true;
	for (int i = 0; i < 10; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[static_cast<int>(IsHorizontal)]);
		m_pShader->activeShader();
		m_pShader->setIntUniformValue("u_IsHorizontal", IsHorizontal);

		if (IsFirst)
			glBindTexture(GL_TEXTURE_2D, m_InputTexture);
		else
			glBindTexture(GL_TEXTURE_2D, m_FBOTexture[static_cast<int>(!IsHorizontal)]);
		
		drawQuad();

		IsHorizontal = !IsHorizontal;
		IsFirst = false;
	}
	//glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CGaussianBlurPass::__calculateGaussianWeightSet()
{
	float Weight = 1.0, PI = 3.1415926;
	int Offset = 0;
	float Sqrt2PI = std::sqrt(2.0 * PI);
	float SigmaSquare = m_GaussianSigma * m_GaussianSigma;
	while (Weight > 0.0001)
	{
		Weight = 1.0 / (Sqrt2PI * m_GaussianSigma) * std::exp(-(Offset * Offset) / (2 * SigmaSquare));
		m_GaussianWeightSet.push_back(Weight);
		++Offset;
	}
}