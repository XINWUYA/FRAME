#include "ScreenQuadPass.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <string>
#include "Interface.h"
#include "Shader.h"
#include "Common.h"
#include "Utils.h"
#include <GLFW/glfw3.h>

CScreenQuadPass::CScreenQuadPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CScreenQuadPass::~CScreenQuadPass()
{
}

//************************************************************************************
//Function:
void CScreenQuadPass::initV()
{
	m_VAO = ElayGraphics::ResourceManager::getOrCreateScreenQuadVAO();
	m_pShader = std::make_shared<CShader>("ScreenQuad_VS.glsl", "ScreenQuad_FS.glsl");
	ElayGraphics::STexture TextureArray;
	int Width = 0, Height = 0, Channels = 0;
	GLvoid* pData = nullptr;
	for (int i = 0; i < 5; ++i)
	{
		stbi_set_flip_vertically_on_load(true);
		pData = stbi_load(("Textures/" + std::to_string(i) + ".jpg").c_str(), &Width, &Height, &Channels, 0);
		TextureArray.pDataSet.push_back(pData);
	}
	TextureArray.TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	TextureArray.InternalFormat = TextureArray.ExternalFormat = GL_RGB;
	TextureArray.Width = Width;
	TextureArray.Height = Height;
	TextureArray.Depth = 5;
	TextureArray.TextureName = "TextureArray";
	m_TextureArray = genTexture(TextureArray);
	for (auto& Item : TextureArray.pDataSet)
		stbi_image_free(Item);
}

//************************************************************************************
//Function:
void CScreenQuadPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShader->activeShader();
	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_RIGHT) == GLFW_PRESS && m_OldKeyRightStatus != GLFW_PRESS)
	{
		m_OldKeyRightStatus = GLFW_PRESS;
		m_Layer = (m_Layer + 1) % 5;
		m_pShader->setIntUniformValue("u_Layer", m_Layer);
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_RIGHT) == GLFW_RELEASE)
		m_OldKeyRightStatus = GLFW_RELEASE;
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_TextureArray);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}