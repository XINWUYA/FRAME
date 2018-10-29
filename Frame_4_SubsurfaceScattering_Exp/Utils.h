#pragma once
#include <GL/glew.h>
#include <vector>
#include "common.h"

#define SAFE_DELETE(p) if(p){ delete p; p = nullptr; }

GLint createVAO(const GLvoid* vVertices, GLint vVerticesSize, std::initializer_list<GLint> vAttribsLength, GLint vStride, const GLint vIndices[] = nullptr, GLint vIndicesSize = 0);
GLint genBuffer(GLenum vTarget, GLsizeiptr vSize, const GLvoid *vData, GLenum vUsage, GLint vBindingIndex = -1);
GLint genTexture2D(ElayGraphics::STexture2D& vioTexture2D/* = ElayGraphics::STexture2D()*/);
GLint loadTextureFromFile(const std::string& vFilePath, ElayGraphics::STexture2D& vioTexture2D = ElayGraphics::STexture2D());
GLint createVAO4ScreenQuad();
GLint genFBO(const std::initializer_list<ElayGraphics::STexture2D>& vTextureAttachments);
void  transferData2Buffer(GLenum vTarget, GLint vTargetID, std::vector<GLintptr> vOffsets, std::vector<GLsizeiptr> vSizes, std::vector<const GLvoid*> vDatas);