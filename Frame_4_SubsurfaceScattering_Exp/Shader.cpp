#include "Shader.h"
#include <crtdbg.h>
#include <fstream>
#include <sstream>
#include <iostream>

CShader::CShader(const std::string& vVertexShaderFileName, const std::string& vFragmentShaderFileName, const std::string& vGeometryShaderFileName,
	const std::string& vTessellationControlShaderFileName, const std::string& vTessellationEvaluationShaderFileName) : m_ShaderProgram(0)
{
	__loadShader(vVertexShaderFileName, vFragmentShaderFileName, vGeometryShaderFileName, vTessellationControlShaderFileName, vTessellationEvaluationShaderFileName);
}

CShader::~CShader()
{
	if (m_ShaderProgram)
		glDeleteProgram(m_ShaderProgram);
}

//************************************************************************************
//Function:
GLvoid CShader::__loadShader(const std::string& vVertexShaderFileName, const std::string& vFragmentShaderFileName, const std::string& vGeometryShaderFileName,
	const std::string& vTessellationControlShaderFileName, const std::string& vTessellationEvaluationShaderFileName)
{
	_ASSERT(!vVertexShaderFileName.empty() && !vFragmentShaderFileName.empty());
	if (m_ShaderProgram)
		return;
	m_ShaderProgram = glCreateProgram();

	GLint VertexShader   = __loadShader(vVertexShaderFileName, GL_VERTEX_SHADER);
	GLint FragmentShader = __loadShader(vFragmentShaderFileName, GL_FRAGMENT_SHADER);

	GLint GeometryShader = 0, TessellationControlShader = 0, TessellationEvaluationShader = 0;
	if (!vGeometryShaderFileName.empty())
		GeometryShader = __loadShader(vGeometryShaderFileName, GL_GEOMETRY_SHADER);
	if (!vTessellationControlShaderFileName.empty())
		TessellationControlShader = __loadShader(vTessellationControlShaderFileName, GL_TESS_CONTROL_SHADER);
	if (!vTessellationEvaluationShaderFileName.empty())
		TessellationEvaluationShader = __loadShader(vTessellationEvaluationShaderFileName, GL_TESS_EVALUATION_SHADER);

	__linkProgram();

	__deleteShader(VertexShader);
	__deleteShader(FragmentShader);
	__deleteShader(GeometryShader);
	__deleteShader(TessellationControlShader);
	__deleteShader(TessellationEvaluationShader);
}

//************************************************************************************
//Function:
GLint CShader::__loadShader(const std::string& vShaderFileName, GLint vShaderType) const
{
	_ASSERT(!vShaderFileName.empty() && vShaderType);
	GLint Shader = glCreateShader(vShaderType);
	_ASSERT(Shader);
	const std::string ShaderSource = __loadShaderSourceFromFile(vShaderFileName);
	const GLchar *pShaderSource = ShaderSource.c_str();
	_ASSERT(pShaderSource);
	glShaderSource(Shader, 1, &pShaderSource, nullptr);
	_ASSERT(__compileShader(Shader));
	_ASSERT(m_ShaderProgram);
	glAttachShader(m_ShaderProgram, Shader);
	return Shader;
}

//************************************************************************************
//Function:
std::string CShader::__loadShaderSourceFromFile(const std::string& vShaderFileName) const
{
	try
	{
		std::ifstream Fin(vShaderFileName);
		std::stringstream FinStream;
		FinStream << Fin.rdbuf();
		return FinStream.str();
	}
	catch(std::exception e)
	{
		std::cerr << "Error::Shader:: Read Shader File Failure: " << vShaderFileName << std::endl;
		return std::string();
	}
}

//************************************************************************************
//Function:
GLboolean CShader::__compileShader(GLint vShader) const
{
	_ASSERT(vShader);
	GLint Success = 0;
	const GLint MaxInfoLogLength = 512;
	GLchar InfoLog[MaxInfoLogLength] = {};
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(vShader, MaxInfoLogLength, nullptr, InfoLog);
		std::cerr << "Error::Shader:: Shader Compile Failure: " << InfoLog << std::endl;
		return GL_FALSE;
	}
	return GL_TRUE;
}

//************************************************************************************
//Function:
GLboolean CShader::__linkProgram() const
{
	_ASSERT(m_ShaderProgram);
	GLint Success = 0;
	const GLint MaxInfoLogLength = 512;
	GLchar InfoLog[MaxInfoLogLength] = {};
	glLinkProgram(m_ShaderProgram);
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(m_ShaderProgram, MaxInfoLogLength, nullptr, InfoLog);
		std::cerr << "Error::Shader:: Shader Program Link Failure: " << InfoLog << std::endl;
		return GL_FALSE;
	}
	return GL_TRUE;
}

//************************************************************************************
//Function:
GLvoid CShader::__deleteShader(GLint vShader) const
{
	if (vShader)
		glDeleteShader(vShader);
}

//************************************************************************************
//Function:
GLvoid CShader::setIntUniformValue(const std::string& vUniformName, GLint v0) const 
{
	glUniform1i(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0);
}

//************************************************************************************
//Function:
GLvoid CShader::setIntUniformValue(const std::string& vUniformName, GLint v0, GLint v1) const
{
	glUniform2i(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1);
}

//************************************************************************************
//Function:
GLvoid CShader::setIntUniformValue(const std::string& vUniformName, GLint v0, GLint v1, GLint v2) const
{
	glUniform3i(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1, v2);
}

//************************************************************************************
//Function:
GLvoid CShader::setIntUniformValue(const std::string& vUniformName, GLint v0, GLint v1, GLint v2, GLint v3) const
{
	glUniform4i(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1, v2, v3);
}

//************************************************************************************
//Function:
GLvoid CShader::setFloatUniformValue(const std::string& vUniformName, GLfloat v0) const
{
	glUniform1f(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0);
}

//************************************************************************************
//Function:
GLvoid CShader::setFloatUniformValue(const std::string& vUniformName, GLfloat v0, GLfloat v1) const
{
	glUniform2f(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1);
}

//************************************************************************************
//Function:
GLvoid CShader::setFloatUniformValue(const std::string& vUniformName, GLfloat v0, GLfloat v1, GLfloat v2) const
{
	glUniform3f(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1, v2);
}

//************************************************************************************
//Function:
GLvoid CShader::setFloatUniformValue(const std::string& vUniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const
{
	glUniform4f(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1, v2, v3);
}

//************************************************************************************
//Function:
GLvoid CShader::setDoubleUniformValue(const std::string& vUniformName, GLdouble v0) const
{
	glUniform1d(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0);
}

//************************************************************************************
//Function:
GLvoid CShader::setDoubleUniformValue(const std::string& vUniformName, GLdouble v0, GLdouble v1) const
{
	glUniform2d(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1);
}

//************************************************************************************
//Function:
GLvoid CShader::setDoubleUniformValue(const std::string& vUniformName, GLdouble v0, GLdouble v1, GLdouble v2) const
{
	glUniform3d(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1, v2);

}

//************************************************************************************
//Function:
GLvoid CShader::setDoubleUniformValue(const std::string& vUniformName, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) const
{
	glUniform4d(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), v0, v1, v2, v3);
}

//************************************************************************************
//Function:
GLvoid CShader::setMat4UniformValue(const std::string& vUniformName, const GLfloat vMatrix[16]) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, vUniformName.c_str()), 1, GL_FALSE, vMatrix);
}

//************************************************************************************
//Function:
GLvoid CShader::setTexture2DUniformValue(const std::string& vTextureName, GLint vTextureID, GLint vBindingIndex) const
{
	glBindTexture(GL_TEXTURE_2D, vTextureID);
	glActiveTexture(GL_TEXTURE0 + vBindingIndex);
	glUniform1i(glGetUniformLocation(m_ShaderProgram, vTextureName.c_str()), vBindingIndex);
}

//************************************************************************************
//Function:
GLvoid CShader::activeShader() const
{
	glUseProgram(m_ShaderProgram);
}