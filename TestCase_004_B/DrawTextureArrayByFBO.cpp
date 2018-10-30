#include "DrawTextureArrayByFBO.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"

CDrawTextureArrayByFBO::CDrawTextureArrayByFBO(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CDrawTextureArrayByFBO::~CDrawTextureArrayByFBO()
{
}

//************************************************************************************
//Function:
void CDrawTextureArrayByFBO::initV()
{
	m_VAO = ElayGraphics::ResourceManager::getOrCreateScreenQuadVAO();
	m_pShader = std::make_shared<CShader>("DrawTextureArrayByFBO_VS.glsl", "DrawTextureArrayByFBO_FS.glsl");

	ElayGraphics::STexture InputTextureArray;
	int Width = 0, Height = 0, Channels = 0;
	GLvoid* pData = nullptr;
	for (int i = 0; i < 5; ++i)
	{
		stbi_set_flip_vertically_on_load(true);
		pData = stbi_load(("Textures/" + std::to_string(i) + ".jpg").c_str(), &Width, &Height, &Channels, 0);
		InputTextureArray.pDataSet.push_back(pData);
	}
	InputTextureArray.TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	InputTextureArray.InternalFormat = InputTextureArray.ExternalFormat = GL_RGB;
	InputTextureArray.Width = Width;
	InputTextureArray.Height = Height;
	InputTextureArray.Depth = 5;
	m_InputTextureArray = genTexture(InputTextureArray);
	for (auto& Item : InputTextureArray.pDataSet)
		stbi_image_free(Item);

	ElayGraphics::STexture FBOTextureArray;
	FBOTextureArray.TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	FBOTextureArray.InternalFormat = FBOTextureArray.ExternalFormat = GL_RGB;
	FBOTextureArray.Width = ElayGraphics::WINDOW_KEYWORD::getWindowWidth();
	FBOTextureArray.Height = ElayGraphics::WINDOW_KEYWORD::getWindowHeight();
	FBOTextureArray.Depth = 5;
	m_FBOTextureArray = genTexture(FBOTextureArray);
	
	m_FBO = genFBO({ FBOTextureArray });
	
	ElayGraphics::ResourceManager::registerSharedData("FBOTextureArray", m_FBOTextureArray);
}

//************************************************************************************
//Function:
void CDrawTextureArrayByFBO::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glViewport(0, 0, m_Width, m_Height);

	m_pShader->activeShader();
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_InputTextureArray);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	//glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}