#pragma once
#include <GL/glew.h>
#include <string>
#include "FRAME_EXPORTS.h"

class FRAME_DLLEXPORTS CShader
{
public:
	CShader() = default;
	CShader(const std::string& vVertexShaderFileName, const std::string& vFragmentShaderFileName, const std::string& vGeometryShaderFileName = "",
		const std::string& vTessellationControlShaderFileName = "", const std::string& vTessellationEvaluationShaderFileName = "");
	~CShader();

	GLvoid setIntUniformValue(const std::string& vUniformName, GLint v0) const;
	GLvoid setIntUniformValue(const std::string& vUniformName, GLint v0, GLint v1) const;
	GLvoid setIntUniformValue(const std::string& vUniformName, GLint v0, GLint v1, GLint v2) const;
	GLvoid setIntUniformValue(const std::string& vUniformName, GLint v0, GLint v1, GLint v2, GLint v3) const;
	
	GLvoid setFloatUniformValue(const std::string& vUniformName, GLfloat v0) const;
	GLvoid setFloatUniformValue(const std::string& vUniformName, GLfloat v0, GLfloat v1) const;
	GLvoid setFloatUniformValue(const std::string& vUniformName, GLfloat v0, GLfloat v1, GLfloat v2) const;
	GLvoid setFloatUniformValue(const std::string& vUniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;

	GLvoid setDoubleUniformValue(const std::string& vUniformName, GLdouble v0) const;
	GLvoid setDoubleUniformValue(const std::string& vUniformName, GLdouble v0, GLdouble v1) const;
	GLvoid setDoubleUniformValue(const std::string& vUniformName, GLdouble v0, GLdouble v1, GLdouble v2) const;
	GLvoid setDoubleUniformValue(const std::string& vUniformName, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) const;

	GLvoid setTextureUniformValue(const std::string& vTextureName, GLint vTextureID, GLint vBindingIndex, GLint vTextureType = GL_TEXTURE_2D) const;
	GLvoid setMat4UniformValue(const std::string& vUniformName, const GLfloat vMatrix[16]) const;

	GLvoid activeShader() const;

private:
	GLvoid    __loadShader(const std::string& vVertexShaderFileName, const std::string& vFragmentShaderFileName, const std::string& vGeometryShaderFileName = "",
						const std::string& vTessellationControlShaderFileName = "", const std::string& vTessellationEvaluationShaderFileName = "");
	GLint    __loadShader(const std::string& vShaderFileName, GLint vShaderType) const;
	GLboolean __compileShader(GLint vShader) const;
	GLboolean __linkProgram() const;
	GLvoid    __deleteShader(GLint vShader) const;

	std::string __loadShaderSourceFromFile(const std::string& vShaderFileName) const;

	GLint m_ShaderProgram;
};