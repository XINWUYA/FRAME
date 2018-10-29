#pragma once
#include <GL/glew.h>
#include <string>
//#include "Utils.h"

namespace ElayGraphics
{
	namespace WINDOW_KEYWORD
	{
		const int WINDOW_WIDTH = 800;
		const int WINDOW_HEIGHT = 600;
		const int VIEWPORT_WIDTH = WINDOW_WIDTH;
		const int VIEWPORT_HEIGHT = WINDOW_HEIGHT;
		const int VIEWPORT_LEFTBOTTOM_X = 0;
		const int VIEWPORT_LEFTBOTTOM_Y = 0;
		const int NUM_SAMPLES = 4;

		const bool CURSOR_DISABLE = true;
		
		const std::string WINDOW_TITLE = "Love Forever";
	}

	struct STexture2D
	{
		enum ETextureType
		{
			ColorTexture,
			DepthTexture,
			StencilTexture,
			DepthAndStencilTexture
		};
		GLint		 InternalFormat;
		GLenum		 ExternalFormat;
		GLenum		 DataType;
		GLsizei		 Width;
		GLsizei		 Height;
		GLvoid*		 pData;
		GLint		 Type4WrapS;
		GLint		 Type4WrapT;
		GLint		 Type4MinFilter;
		GLint		 Type4MagFilter;
		GLboolean	 isMipmap;
		GLint		 TextureID;
		std::string  TextureName;
		ETextureType TextureType;

		STexture2D():TextureID(-1),InternalFormat(GL_RGBA),ExternalFormat(GL_RGBA),DataType(GL_UNSIGNED_BYTE),Width(WINDOW_KEYWORD::WINDOW_WIDTH),
			Height(WINDOW_KEYWORD::WINDOW_HEIGHT),pData(nullptr),Type4WrapS(GL_REPEAT),Type4WrapT(GL_REPEAT),Type4MinFilter(GL_LINEAR),
			Type4MagFilter(GL_LINEAR),isMipmap(GL_TRUE),TextureType(ETextureType::ColorTexture)
		{}
	};
}