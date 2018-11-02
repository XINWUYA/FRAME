#include "SpherePBRPass.h"
#include <GLM/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Interface.h"
#include "Utils.h"
#include "SphereSet.h"

CSpherePass::CSpherePass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName,vExecutionOrder)
{
}

CSpherePass::~CSpherePass()
{
}

//************************************************************************************
//Function:
void CSpherePass::initV()
{
	m_IrradianceCubeMap = ElayGraphics::ResourceManager::getSharedDataByName<int>("IrradianceCubeTexture");
	m_RowNum = ElayGraphics::ResourceManager::getSharedDataByName<int>("SphereRowNum");
	m_ColNum = ElayGraphics::ResourceManager::getSharedDataByName<int>("SphereColNum");
	m_SphereSet = std::dynamic_pointer_cast<CSphereSet>(ElayGraphics::ResourceManager::getGameObjectByName("SphereSet"));
	m_IrradianceCubeMap = ElayGraphics::ResourceManager::getSharedDataByName<int>("IrradianceCubeTexture");
	m_PrefilterEnvMap = ElayGraphics::ResourceManager::getSharedDataByName<int>("PrefilterEnvMap");
	m_BRDFMap = ElayGraphics::ResourceManager::getSharedDataByName<int>("BRDFMap");

	glm::vec3 LightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 LightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};

	m_pShader = std::make_shared<CShader>("SpherePBR_VS.glsl", "SpherePBR_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_ObjectColor", 0.5f, 0.0f, 0.0f);
	m_pShader->setFloatUniformValue("u_AO", 1.0f);
	m_pShader->setTexture2DUniformValue("u_IrradianceMap", m_IrradianceCubeMap, 0);
	m_pShader->setTexture2DUniformValue("u_PrefilterEnvMap", m_PrefilterEnvMap, 1);
	m_pShader->setTexture2DUniformValue("u_BRDFMap", m_BRDFMap, 2);
	for (int i = 0; i < sizeof(LightPositions) / sizeof(LightPositions[0]); ++i)
	{
		glm::vec3 NewPos = LightPositions[i];
		m_pShader->setFloatUniformValue("u_LightPos[" + std::to_string(i) + "]", NewPos[0], NewPos[1], NewPos[2]);
		m_pShader->setFloatUniformValue("u_LightColors[" + std::to_string(i) + "]", LightColors[i][0], LightColors[i][1], LightColors[i][2]);
	}
}

//************************************************************************************
//Function:
void CSpherePass::updateV()
{
	m_pShader->activeShader();
	const glm::vec3& CameraPos = ElayGraphics::Camera::getMainCameraPos();
	m_pShader->setFloatUniformValue("u_ViewPos", CameraPos[0], CameraPos[1], CameraPos[2]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceCubeMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterEnvMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_BRDFMap);
	float Spacing = 2.5f;
	for (int Row = 0; Row < m_RowNum; ++Row)
	{
		m_pShader->setFloatUniformValue("u_Metalness", float(Row) / m_RowNum);
		for (int Col = 0; Col < m_ColNum; ++Col)
		{
			m_pShader->setFloatUniformValue("u_Roughness", glm::clamp(float(Col) / m_ColNum, 0.1f, 1.0f));
			m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_SphereSet->getModelMatrixBySphereIndex(Row, Col)));
			drawSphere();
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}