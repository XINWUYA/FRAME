#include "ModelPass.h"
#include <GLM/gtc/type_ptr.hpp>
#include "Interface.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "LightSource.h"
#include "Utils.h"
#include <GLFW/glfw3.h>

CModelPass::CModelPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CModelPass::~CModelPass()
{
}

void CModelPass::initV()
{
	m_pShader = std::make_shared<CShader>("CornelBox_VS.glsl", "CornelBox_FS.glsl");
	m_LTCMatrixTexture = loadTextureFromFile("../Textures/LTCLight/ltc_mat.dds");
	m_LTCMagnitueTexture = loadTextureFromFile("../Textures/LTCLight/ltc_amp.dds");
	m_pLightSource = std::dynamic_pointer_cast<CLightSource>(ElayGraphics::ResourceManager::getGameObjectByName("LightSource"));
	m_pGameObject = ElayGraphics::ResourceManager::getGameObjectByName("CornelBox");
	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pGameObject->getModelMatrix()));
}

void CModelPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	m_pShader->activeShader();
	bool DiffuseColorChanged = false;
	m_Albedo = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec4>("Albedo", m_Albedo, DiffuseColorChanged);
	if (DiffuseColorChanged)
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

	const glm::mat4& GroundModelMatrix = m_pGameObject->getModelMatrix();
	if (GroundModelMatrix != m_GameObjectModelMatrix)
	{
		m_GameObjectModelMatrix = GroundModelMatrix;
		m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_GameObjectModelMatrix));
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

	m_pGameObject->updateModel(*m_pShader);

	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}