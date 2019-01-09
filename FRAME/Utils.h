#pragma once
#include <GL/glew.h>
#include <vector>
#include "FRAME_EXPORTS.h"
#include "common.h"

#define SAFE_DELETE(p) if(p){ delete p; p = nullptr; }

FRAME_DLLEXPORTS GLint createVAO(const GLvoid* vVertices, GLint vVerticesSize, std::initializer_list<GLint> vAttribsLength, const GLint vIndices[] = nullptr, GLint vIndicesSize = 0, int *voVBO = nullptr);
FRAME_DLLEXPORTS GLint genBuffer(GLenum vTarget, GLsizeiptr vSize, const GLvoid *vData, GLenum vUsage, GLint vBindingIndex = -1);
FRAME_DLLEXPORTS GLint genTexture(ElayGraphics::STexture& vioTexture/* = ElayGraphics::STexture()*/);
FRAME_DLLEXPORTS GLint loadTextureFromFile(const std::string& vFilePath, ElayGraphics::STexture& voTexture2D = ElayGraphics::STexture());
FRAME_DLLEXPORTS GLint createVAO4ScreenQuad();
FRAME_DLLEXPORTS GLint createVAO4Cube();
FRAME_DLLEXPORTS GLint createVAO4Sphere();
FRAME_DLLEXPORTS void  drawQuad();
FRAME_DLLEXPORTS void  drawCube();
FRAME_DLLEXPORTS void  drawSphere();
FRAME_DLLEXPORTS GLint genFBO(const std::initializer_list<ElayGraphics::STexture>& vTextureAttachments);
FRAME_DLLEXPORTS void  transferData2Buffer(GLenum vTarget, GLint vTargetID, std::vector<GLintptr> vOffsets, std::vector<GLsizeiptr> vSizes, std::vector<const GLvoid*> vDatas);
FRAME_DLLEXPORTS int captureScreen2Img(const std::string& vFileName, int vQuality = 100.0);

bool floatEqual(float vFloatNum1, float vFloatNum2);