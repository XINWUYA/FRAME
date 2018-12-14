#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <tuple>
#include <map>
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

	GLvoid setTextureUniformValue(const std::string& vTextureUniformName, GLint vTextureID, GLint vTextureType = GL_TEXTURE_2D);
	GLvoid setMat4UniformValue(const std::string& vUniformName, const GLfloat vMatrix[16]) const;

	GLvoid changeTextureUniformValue(const std::string& vTextureUniformName, GLint vTextureID, GLint vTextureType/* = GL_TEXTURE_2D*/);

	GLvoid activeShader() const;

private:
	GLint m_ShaderProgram = 0;
	GLint m_LastBindingIndex = 4;
	std::map<std::string, std::tuple<int, int, int>> m_TextureNameAndTextureIDAndBindingIndexAndTextureTypeSet;
	//std::vector<std::tuple<int, int, int>> m_TextureAndBindingIndexAndTextureTypeSet;

	GLint		__loadShader(const std::string& vShaderFileName, GLint vShaderType) const;
	GLvoid		__loadShader(const std::string& vVertexShaderFileName, const std::string& vFragmentShaderFileName, const std::string& vGeometryShaderFileName = "",
								const std::string& vTessellationControlShaderFileName = "", const std::string& vTessellationEvaluationShaderFileName = "");
	GLvoid		__deleteShader(GLint vShader) const;
	GLboolean	__compileShader(GLint vShader) const;
	GLboolean	__linkProgram() const;
	std::string __loadShaderSourceFromFile(const std::string& vShaderFileName) const;

	GLvoid		__activeAllTextureUniform() const;
};