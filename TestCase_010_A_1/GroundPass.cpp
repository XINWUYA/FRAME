#include "GroundPass.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <memory>
#include "Utils.h"
#include "Shader.h"
#include "Interface.h"
#include "GameObject.h"
#include "LightSource.h"
#include "Ground.h"
#include <GLFW/glfw3.h>

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
	m_LTCMatrixTexture = loadTextureFromFile("../Textures/LTCLight/ltc_mat.dds");
	m_LTCMagnitueTexture = loadTextureFromFile("../Textures/LTCLight/ltc_amp.dds");
	m_LTC_DisneyDiffuse_MatrixTexture = loadTextureFromFile("../Textures/LTCLight/ltc_DisneyDiffuse_NoPI_N32_mat.dds");
	m_RoughnessTexture = loadTextureFromFile("Sponza_Floor_roughness.tga");
	m_pLightSource = std::dynamic_pointer_cast<CLightSource>(ElayGraphics::ResourceManager::getGameObjectByName("LightSource"));
	m_pGround = std::dynamic_pointer_cast<CGround>(ElayGraphics::ResourceManager::getGameObjectByName("Ground"));

	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_LTC_MatrixTexture", m_LTCMatrixTexture);
	m_pShader->setTextureUniformValue("u_LTC_MagnitueTexture", m_LTCMagnitueTexture);
	m_pShader->setTextureUniformValue("u_LTC_DisneyDiffuse_MatrixTexture", m_LTC_DisneyDiffuse_MatrixTexture);
	m_pShader->setTextureUniformValue("u_RoughnessTexture", m_RoughnessTexture);
}

//************************************************************************************
//Function:
void CGroundPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShader->activeShader();

	bool DiffuseColorChanged = false;
	m_Albedo = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec4>("Albedo", m_Albedo, DiffuseColorChanged);
	if(DiffuseColorChanged)
		m_pShader->setFloatUniformValue("u_Albedo", m_Albedo[0], m_Albedo[1], m_Albedo[2]);

	bool RoughnessChanged = false;
	m_Roughness = ElayGraphics::ResourceManager::getSharedDataByName<float>("Roughness", m_Roughness, RoughnessChanged);
	if (RoughnessChanged)
		m_pShader->setFloatUniformValue("u_Roughness", m_Roughness);

	bool MetalnessChanged = false;
	m_Metalness = ElayGraphics::ResourceManager::getSharedDataByName<float>("Metalness", m_Metalness, MetalnessChanged);
	if (MetalnessChanged)
		m_pShader->setFloatUniformValue("u_Metalness", m_Metalness);

	bool IntensityChanged = false;
	m_Intensity = ElayGraphics::ResourceManager::getSharedDataByName<float>("Intensity", m_Intensity, IntensityChanged);
	if (IntensityChanged)
		m_pShader->setFloatUniformValue("u_Intensity", m_Intensity);

	const glm::vec3& LightPos = m_pLightSource->getLightPos();
	if (LightPos != m_LightSourcePos)
	{
		m_LightSourcePos = LightPos;
		m_pShader->setFloatUniformValue("u_LightPosition", m_LightSourcePos.x, m_LightSourcePos.y, m_LightSourcePos.z);
	}

	const glm::mat4& GroundModelMatrix = m_pGround->getModelMatrix();
	if (GroundModelMatrix != m_GroundModelMatrix)
	{
		m_GroundModelMatrix = GroundModelMatrix;
		m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_GroundModelMatrix));
	}

	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_PRESS && m_OldKeyPStatus != GLFW_PRESS)
	{
		m_OldKeyPStatus = GLFW_PRESS;
		m_EnableLTC = !m_EnableLTC;
		m_pShader->setIntUniformValue("u_EnableLTC", m_EnableLTC);
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_RELEASE)
		m_OldKeyPStatus = GLFW_RELEASE;

	glm::vec3 CameraPos = ElayGraphics::Camera::getMainCameraPos();
	m_pShader->setFloatUniformValue("u_CameraPosInWorldSpace", CameraPos.x, CameraPos.y, CameraPos.z);
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}