#include "GroundPass.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <memory>
#include "Utils.h"
#include "Shader.h"
#include "LightSource.h"
#include "Interface.h"

CGroundPass::CGroundPass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

CGroundPass::~CGroundPass()
{
}

//************************************************************************************
//Function:
void CGroundPass::initV()
{
	m_pShader = std::make_shared<CShader>("Ground_VS.glsl", "Ground_FS.glsl");
	m_LTCMatrixTexture = loadTextureFromFile("Textures/ltc_mat.dds");
	m_LTCMagnitueTexture = loadTextureFromFile("Textures/ltc_amp.dds");

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(ElayGraphics::ResourceManager::getGameObjectByName("Ground")->getModelMatrix()));
	m_pShader->setTexture2DUniformValue("u_LTC_MatrixTexture", m_LTCMatrixTexture, 0);
	m_pShader->setTexture2DUniformValue("u_LTC_MagnitueTexture", m_LTCMagnitueTexture, 1);
}

//************************************************************************************
//Function:
void CGroundPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShader->activeShader();

	glm::vec4 DiffuseColorFromGUI = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec4>("DiffuseColor");
	if (DiffuseColorFromGUI != m_DiffuseColor)
	{
		m_DiffuseColor = DiffuseColorFromGUI;
		m_pShader->setFloatUniformValue("u_DiffuseColor", m_DiffuseColor[0], m_DiffuseColor[1], m_DiffuseColor[2]);
	}

	glm::vec4 SpecularColorFromGUI = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec4>("SpecularColor");
	if (SpecularColorFromGUI != m_SpecularColor)
	{
		m_SpecularColor = SpecularColorFromGUI;
		m_pShader->setFloatUniformValue("u_SpecularColor", m_SpecularColor[0], m_SpecularColor[1], m_SpecularColor[2]);
	}

	float RoughnessFromGUI = ElayGraphics::ResourceManager::getSharedDataByName<float>("Roughness");
	if (RoughnessFromGUI != m_Roughness)
	{
		m_Roughness = RoughnessFromGUI;
		m_pShader->setFloatUniformValue("u_Roughness", m_Roughness);
	}

	float Intensity = ElayGraphics::ResourceManager::getSharedDataByName<float>("Intensity");
	if (Intensity != m_Intensity)
	{
		m_Intensity = Intensity;
		m_pShader->setFloatUniformValue("u_Intensity", m_Intensity);
	}

	bool IsTwoSide = ElayGraphics::ResourceManager::getSharedDataByName<bool>("IsTwoSide");
	if (IsTwoSide != m_IsTwoSide)
	{
		m_IsTwoSide = IsTwoSide;
		m_pShader->setIntUniformValue("u_IsTwoSided", m_IsTwoSide);
	}

	const auto &PolygonalLightVertexPos = std::dynamic_pointer_cast<CLightSource>(ElayGraphics::ResourceManager::getGameObjectByName("LightSource"))->getPolygonalLightVertexPosSet();
	if (PolygonalLightVertexPos != m_PolygonalLightVertexPos)
	{
		m_PolygonalLightVertexPos = PolygonalLightVertexPos;
		for (int i = 0; i < 4; ++i)
		{
			m_pShader->setFloatUniformValue("u_PolygonalLightVertexPos[" + std::to_string(i) + "]", PolygonalLightVertexPos[i].x, PolygonalLightVertexPos[i].y, PolygonalLightVertexPos[i].z);
		}
	}

	glm::vec3 CameraPos = ElayGraphics::Camera::getMainCameraPos();
	m_pShader->setFloatUniformValue("u_CameraPosInWorldSpace", CameraPos.x, CameraPos.y, CameraPos.z);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_LTCMatrixTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_LTCMagnitueTexture);
	glBindVertexArray(ElayGraphics::ResourceManager::getGameObjectByName("Ground")->getVAO());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}