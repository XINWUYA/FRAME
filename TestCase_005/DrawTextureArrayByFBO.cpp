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
	for (int i = 0; i < 8; ++i)
	{
		stbi_set_flip_vertically_on_load(true);
		//pData = stbi_load("Textures/0.jpg", &Width, &Height, &Channels, 0);
		//pData = stbi_load("Textures/white.png", &Width, &Height, &Channels, 0);
		pData = stbi_load(("Textures/" + std::to_string(i) + ".png").c_str(), &Width, &Height, &Channels, 0);
		InputTextureArray.pDataSet.push_back(pData);
	}
	InputTextureArray.TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	InputTextureArray.InternalFormat = InputTextureArray.ExternalFormat = (Channels == 4) ? GL_RGBA : GL_RGB;
	InputTextureArray.Width = Width;
	InputTextureArray.Height = Height;
	InputTextureArray.Depth = 8;
	m_InputTextureArray = genTexture(InputTextureArray);
	for (auto& Item : InputTextureArray.pDataSet)
		stbi_image_free(Item);

	ElayGraphics::STexture FBOTextureArray;
	FBOTextureArray.TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	FBOTextureArray.InternalFormat = FBOTextureArray.ExternalFormat = GL_RGB;
	FBOTextureArray.Width = ElayGraphics::WINDOW_KEYWORD::getWindowWidth();
	FBOTextureArray.Height = ElayGraphics::WINDOW_KEYWORD::getWindowHeight();
	FBOTextureArray.Depth = 5;
	FBOTextureArray.Type4WrapS = GL_CLAMP_TO_BORDER;
	FBOTextureArray.Type4WrapT = GL_CLAMP_TO_BORDER;
	FBOTextureArray.BorderColor = { 1,0,0,1 };
	/*FBOTextureArray.Type4WrapS = GL_CLAMP_TO_EDGE;
	FBOTextureArray.Type4WrapT = GL_CLAMP_TO_EDGE;
	FBOTextureArray.Type4MinFilter = GL_LINEAR_MIPMAP_LINEAR;
	FBOTextureArray.Type4MagFilter = GL_LINEAR;*/
	for (int i = 0; i < 2; ++i)
	{
		m_FBOTextureArray[i] = genTexture(FBOTextureArray);
		m_FBO[i] = genFBO({ FBOTextureArray });
	}
	
	//ElayGraphics::ResourceManager::registerSharedData("FBOTextureArray", m_FBOTextureArray[0]);
	ElayGraphics::ResourceManager::registerSharedData("FBOTextureArray", m_InputTextureArray);
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_TextureArray", 0);
}

//************************************************************************************
//Function:
void CDrawTextureArrayByFBO::updateV()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClearColor(1, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	////glViewport(0, 0, m_Width, m_Height);
	//m_pShader->activeShader();
	//glActiveTexture(GL_TEXTURE0);
	//bool IsHorizontal = true;
	//bool IsFirst = true;
	//for (int i = 0; i < 10; ++i)
	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[static_cast<int>(IsHorizontal)]);
	//	m_pShader->setIntUniformValue("u_IsHorizontal", IsHorizontal);

	//	if (IsFirst)
	//		glBindTexture(GL_TEXTURE_2D_ARRAY, m_InputTextureArray);
	//	else
	//		glBindTexture(GL_TEXTURE_2D_ARRAY, m_FBOTextureArray[static_cast<int>(!IsHorizontal)]);
	//	
	//	glBindVertexArray(m_VAO);
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//	glBindVertexArray(0);

	//	static const GLenum DrawBuffers[] = {
	//			GL_COLOR_ATTACHMENT0,
	//			GL_COLOR_ATTACHMENT1,
	//			GL_COLOR_ATTACHMENT2,
	//			GL_COLOR_ATTACHMENT3,
	//			GL_COLOR_ATTACHMENT4,
	//	};
	//	glDrawBuffers(5, DrawBuffers);

	//	IsHorizontal = !IsHorizontal;
	//	IsFirst = false;
	//}
	////glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}