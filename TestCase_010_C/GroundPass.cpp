#include "GroundPass.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <memory>
#include "Utils.h"
#include "Shader.h"
#include "Interface.h"
#include "GameObject.h"
#include "LightSource.h"
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
	m_pLightSource = std::dynamic_pointer_cast<CLightSource>(ElayGraphics::ResourceManager::getGameObjectByName("LightSource"));

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(ElayGraphics::ResourceManager::getGameObjectByName("Ground")->getModelMatrix()));
	m_pShader->setTextureUniformValue("u_LTC_MatrixTexture", m_LTCMatrixTexture, 0);
	m_pShader->setTextureUniformValue("u_LTC_MagnitueTexture", m_LTCMagnitueTexture, 1);

	auto pLightInfo = ElayGraphics::ResourceManager::getSharedDataByName<SLight*>("LightInfo");
	auto LightInfoByteSize = ElayGraphics::ResourceManager::getSharedDataByName<size_t>("LightInfoByteSize");
	genBuffer(GL_SHADER_STORAGE_BUFFER, LightInfoByteSize, pLightInfo, GL_STATIC_DRAW, 1);
	auto LightNum = ElayGraphics::ResourceManager::getSharedDataByName<size_t>("LightNum");
	m_pShader->setIntUniformValue("u_LightNum", LightNum);
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
	m_DiffuseColor = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec4>("DiffuseColor", m_DiffuseColor, DiffuseColorChanged);
	if(DiffuseColorChanged)
		m_pShader->setFloatUniformValue("u_DiffuseColor", m_DiffuseColor[0], m_DiffuseColor[1], m_DiffuseColor[2]);

	bool SpecularColorChanged = false;
	m_SpecularColor = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec4>("SpecularColor", m_SpecularColor, SpecularColorChanged);
	if (SpecularColorChanged)
		m_pShader->setFloatUniformValue("u_SpecularColor", m_SpecularColor[0], m_SpecularColor[1], m_SpecularColor[2]);

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

	glm::vec3 CameraPos = ElayGraphics::Camera::getMainCameraPos();
	m_pShader->setFloatUniformValue("u_CameraPosInWorldSpace", CameraPos.x, CameraPos.y, CameraPos.z);

	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_PRESS && m_OldKeyPStatus != GLFW_PRESS)
	{
		m_OldKeyPStatus = GLFW_PRESS;
		m_EnableLTC = !m_EnableLTC;
		m_pShader->setIntUniformValue("u_EnableLTC", m_EnableLTC);
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_RELEASE)
		m_OldKeyPStatus = GLFW_RELEASE;

	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}