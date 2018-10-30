#include "Utils.h"
#include <initializer_list>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <gli.hpp>
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "common.h"

//************************************************************************************
//Function:
GLint createVAO(const GLvoid* vVertices, GLint vVerticesSize, std::initializer_list<GLint> vAttribsLength, GLint vStride, const GLint vIndices[], GLint vIndicesSize)
{
	GLint VAO = 0, VBO = 0, EBO = 0;
	glGenVertexArrays(1, &(GLuint&)VAO);
	glBindVertexArray(VAO);
	VBO = genBuffer(GL_ARRAY_BUFFER, vVerticesSize, vVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //In function "genBuffer", bindbuffer(0) is called, so here we must rebind so that VAO is effective
	if (vIndices)
	{
		EBO = genBuffer(GL_ELEMENT_ARRAY_BUFFER, vIndicesSize, vIndices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	}
	GLint AttribOffset = 0;
	GLint i = 0;
	for (auto vAttribLength : vAttribsLength)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, vAttribLength, GL_FLOAT, GL_FALSE, vStride * sizeof(GL_FLOAT), (GLvoid*)(AttribOffset * sizeof(GL_FLOAT)));
		AttribOffset += vAttribLength;
		++i;
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return VAO;
}

//************************************************************************************
//Function:
GLint createVAO4ScreenQuad()
{
	GLfloat windowQuadVertices[] = {
		1.0f, 1.0f,  1.0f,1.0f,
		1.0f,-1.0f,  1.0f,0.0f,
		-1.0f, 1.0f,  0.0f,1.0f,
		-1.0f, 1.0f,  0.0f,1.0f,
		1.0f,-1.0f,  1.0f,0.0f,
		-1.0f,-1.0f,  0.0f,0.0f
	};
	return createVAO(windowQuadVertices, sizeof(windowQuadVertices), { 2,2 }, 4);
}

//************************************************************************************
//Function: 
GLint genBuffer(GLenum vTarget, GLsizeiptr vSize, const GLvoid *vData, GLenum vUsage, GLint vBindingIndex)
{
	GLint BufferID;
	glGenBuffers(1, &(GLuint&)BufferID);
	glBindBuffer(vTarget, BufferID);
	glBufferData(vTarget, vSize, vData, vUsage);
	glBindBuffer(vTarget, 0);
	if (vBindingIndex != -1)
		glBindBufferBase(vTarget, vBindingIndex, BufferID);
	return BufferID;
}

//************************************************************************************
//Function:
void transferData2Buffer(GLenum vTarget,GLint vTargetID,std::vector<GLintptr> vOffsets, std::vector<GLsizeiptr> vSizes,std::vector<const GLvoid*> vDatas)
{
	_ASSERT(vOffsets.size() == vSizes.size() && vOffsets.size() == vDatas.size());
	GLint OffsetSize = static_cast<GLint>(vOffsets.size());
	glBindBuffer(vTarget, vTargetID);
	for (auto i = 0; i < OffsetSize; ++i)
	{
		glBufferSubData(vTarget, vOffsets[i], vSizes[i], vDatas[i]);
	}
	glBindBuffer(vTarget, 0);
}

//************************************************************************************
//Function:
GLint genTexture(ElayGraphics::STexture& vioTexture)
{
	GLint TextureID;
	glGenTextures(1, &(GLuint&)TextureID);

	GLuint TextureType = -1;
	switch (vioTexture.TextureType)
	{
	case ElayGraphics::STexture::ETextureType::Texture2D:
		TextureType = GL_TEXTURE_2D;
		glBindTexture(TextureType, TextureID);
		glTexImage2D(TextureType, 0, vioTexture.InternalFormat, vioTexture.Width, vioTexture.Height, 0, vioTexture.ExternalFormat, vioTexture.DataType, vioTexture.pDataSet.size() > 0 ? vioTexture.pDataSet[0] : nullptr);
		break;
	case ElayGraphics::STexture::ETextureType::Texture2DArray:
		TextureType = GL_TEXTURE_2D_ARRAY;
		glBindTexture(TextureType, TextureID);
		glTexImage3D(TextureType, 0, vioTexture.InternalFormat, vioTexture.Width, vioTexture.Height, vioTexture.Depth, 0, vioTexture.ExternalFormat, vioTexture.DataType, nullptr);
		for (size_t i = 0; i < vioTexture.pDataSet.size(); ++i)
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, vioTexture.Width, vioTexture.Height, 1, vioTexture.ExternalFormat, vioTexture.DataType, vioTexture.pDataSet[i]);
		break;
	case ElayGraphics::STexture::ETextureType::Texture3D:
		TextureType = GL_TEXTURE_3D;
		glBindTexture(TextureType, TextureID);
		break;
	case ElayGraphics::STexture::ETextureType::TextureCubeMap:
		TextureType = GL_TEXTURE_CUBE_MAP;
		glBindTexture(TextureType, TextureID);
		break;
	default:
		break;
	}
	glTexParameteri(TextureType, GL_TEXTURE_WRAP_S, vioTexture.Type4WrapS);
	glTexParameteri(TextureType, GL_TEXTURE_WRAP_T, vioTexture.Type4WrapT);
	glTexParameteri(TextureType, GL_TEXTURE_WRAP_R, vioTexture.Type4WrapR);
	glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, vioTexture.Type4MinFilter);
	glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, vioTexture.Type4MagFilter);
	if (vioTexture.isMipmap) glGenerateMipmap(TextureType);
	glBindTexture(TextureType, 0);
	vioTexture.TextureID = TextureID;
	return TextureID;
}

//************************************************************************************
//Function:
GLint loadTextureFromFile(const std::string& vFilePath, ElayGraphics::STexture& vioTexture2D)
{
	GLint ImageWidth = 0, ImageHeight = 0;
	
	std::vector<std::string> SplitStringSet;
	boost::split(SplitStringSet, vFilePath, boost::is_any_of(".")); 
	bool IsUseGLI = false;
	if (*(SplitStringSet.end() - 1) == "dds")
		IsUseGLI = true;

	//GLubyte *pImageData = nullptr;
	GLvoid *pImageData = nullptr;
	GLint ExternalFormat = -1, InternalFormat = -1, DataType = -1;
	gli::texture GLITexture;
	if (IsUseGLI)
	{
		GLITexture = gli::load(vFilePath);
		pImageData = GLITexture.data();
		gli::gl GL(gli::gl::PROFILE_GL33);
		gli::gl::format const GLIFormat = GL.translate(GLITexture.format(), GLITexture.swizzles());
		InternalFormat = GLIFormat.Internal;
		ExternalFormat = GLIFormat.External;
		DataType = GLIFormat.Type;
		ImageWidth = GLITexture.extent().x;
		ImageHeight = GLITexture.extent().y;
	}
	else
	{
		GLint NumChannels = 0;
		stbi_set_flip_vertically_on_load(true);
		pImageData = stbi_load(vFilePath.c_str(), &ImageWidth, &ImageHeight, &NumChannels, 0);
		switch (NumChannels)
		{
		case 1:
			InternalFormat = ExternalFormat = GL_RED;
			break;
		case 2:
			InternalFormat = ExternalFormat = GL_RG;
			break;
		case 3:
			InternalFormat = ExternalFormat = GL_RGB;
			break;
		case 4:
			InternalFormat = ExternalFormat = GL_RGBA;
			break;
		default:
			break;
		}
		DataType = GL_UNSIGNED_BYTE;
	}
	_ASSERT(pImageData);
	
	vioTexture2D.Width = ImageWidth;
	vioTexture2D.Height = ImageHeight;
	vioTexture2D.InternalFormat = InternalFormat;
	vioTexture2D.ExternalFormat = ExternalFormat;
	vioTexture2D.DataType = DataType;
	vioTexture2D.pDataSet.push_back(pImageData);
	GLint TextureID = genTexture(vioTexture2D);

	if (!IsUseGLI) stbi_image_free(pImageData);
	return TextureID;
}

//************************************************************************************
//Function:
GLint genFBO(const std::initializer_list<ElayGraphics::STexture>& vTextureAttachments)
{
	GLint FBO;
	glGenFramebuffers(1, &(GLuint&)FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	GLint i = -1;
	GLboolean HasDepthTextureAttachment = GL_FALSE, HasStencilTextureAttachment = GL_FALSE;
	std::vector<GLenum> Attachments;
	Attachments.reserve(vTextureAttachments.size());
	for (const ElayGraphics::STexture &vTexture : vTextureAttachments)
	{
		_ASSERT(vTexture.TextureID != -1);
		switch (vTexture.TextureAttachmentType)
		{
		case ElayGraphics::STexture::ETextureAttachmentType::DepthTexture:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, vTexture.TextureID, 0);
			HasDepthTextureAttachment = GL_TRUE;
			HasStencilTextureAttachment = GL_TRUE;	//FIXME£ºthere is a problem: if set it as GL_FALSE, the stencil render buffer will be added, then result in incomplete fbo 
			break;
		case ElayGraphics::STexture::ETextureAttachmentType::StencilTexture:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, vTexture.TextureID, 0);
			HasDepthTextureAttachment = GL_TRUE;
			HasStencilTextureAttachment = GL_TRUE;	//FIXME: there is also same problem as above
			break;
		case ElayGraphics::STexture::ETextureAttachmentType::DepthAndStencilTexture:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, vTexture.TextureID, 0);
			HasDepthTextureAttachment = GL_TRUE;
			HasStencilTextureAttachment = GL_TRUE; 
			break;
		default:
			switch (vTexture.TextureType)
			{
			case ElayGraphics::STexture::ETextureType::Texture2D:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (++i), GL_TEXTURE_2D, vTexture.TextureID, 0);
				Attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
				break;
			case ElayGraphics::STexture::ETextureType::Texture2DArray:
				for (int k = 0; k < vTexture.Depth; ++k)
				{
					glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (++i), vTexture.TextureID, 0, k);
					Attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
				}
				break;
			default:
				break;
			}
			HasStencilTextureAttachment = GL_TRUE;
			break;
		}
	}
	if (Attachments.size())
		glDrawBuffers(Attachments.size(), &Attachments[0]);

	auto genRenderBufferFunc = [](GLenum vInternelFormat, GLenum vAttachmentType)
	{
		GLint RenderBuffer;
		glGenRenderbuffers(1, &(GLuint&)RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, vInternelFormat, ElayGraphics::WINDOW_KEYWORD::WINDOW_WIDTH, ElayGraphics::WINDOW_KEYWORD::WINDOW_HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, vAttachmentType, GL_RENDERBUFFER, RenderBuffer);
	};
	if (!HasDepthTextureAttachment)
	{
		genRenderBufferFunc(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	}
	if (!HasStencilTextureAttachment)
	{
		genRenderBufferFunc(GL_STENCIL_INDEX, GL_STENCIL_ATTACHMENT);
	}
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Error::FBO:: Framebuffer Is Not Complete." << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return FBO;
}