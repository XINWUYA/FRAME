#include "GaussianBlurPass.h"
#include "Shader.h"
#include "Utils.h"
#include "Interface.h"

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
	ElayGraphics::STexture InputTexture2D;
	InputTexture2D.isMipmap = GL_TRUE;
	m_InputTexture = loadTextureFromFile("../Textures/GaussianBlurTexture/0.png", InputTexture2D);

	ElayGraphics::STexture Texture2D;
	Texture2D.Width = 512;
	Texture2D.Height = 512;
	Texture2D.InternalFormat = GL_RGBA16F;
	Texture2D.ExternalFormat = GL_RGBA;
	Texture2D.DataType = GL_FLOAT;
	Texture2D.Type4WrapS = Texture2D.Type4WrapT = GL_CLAMP_TO_EDGE;
	Texture2D.isMipmap = GL_TRUE;
	//Texture2D.Type4MinFilter = GL_LINEAR_MIPMAP_LINEAR;
	for (int i = 0; i < 2; ++i)
	{
		m_PingPongTexture[i] = genTexture(Texture2D);
		m_PingPongFBO[i] = genFBO({ Texture2D });
	}
	ElayGraphics::ResourceManager::registerSharedData("BluredTexture", m_PingPongTexture[0]);
	ElayGraphics::ResourceManager::registerSharedData("MaxMipLevel", m_MaxMipLevel);
}

//************************************************************************************
//Function:
void CGaussianBlurPass::updateV()
{
	/*glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	glBindTexture(GL_TEXTURE_2D, m_InputTexture);
	drawQuad();*/
	for (int MipLevel = 0; MipLevel < m_MaxMipLevel; ++MipLevel)
	{
		int MipWidth = 512 * std::pow(0.5, MipLevel);
		int MipHeight = MipWidth;

		bool IsHorizontal = true, IsFirstIteration = true;
		int BlurAmount = 10;
		m_pShader->activeShader();
		m_pShader->setIntUniformValue("u_MipLevel", MipLevel);
		for (int i = 0; i < BlurAmount; ++i)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFBO[IsHorizontal]);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_PingPongTexture[IsHorizontal], MipLevel);
			glViewport(0, 0, MipWidth, MipHeight);
			glClearColor(1, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_pShader->setIntUniformValue("u_IsHorizontal", IsHorizontal);
			glBindTexture(GL_TEXTURE_2D, IsFirstIteration ? m_InputTexture : m_PingPongTexture[!IsHorizontal]);
			drawQuad();
			IsHorizontal = !IsHorizontal;
			if (IsFirstIteration)
				IsFirstIteration = false;
		}
	}
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
}