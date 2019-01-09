#include "SponzaPass.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <memory>
#include "Utils.h"
#include "Shader.h"
#include "Interface.h"
#include "GameObject.h"
#include "LightSource.h"
#include "Sponza.h"
#include <GLFW/glfw3.h>

CSponzaPass::CSponzaPass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

CSponzaPass::~CSponzaPass()
{
	//m_CameraPosFileOut.close();
}

//************************************************************************************
//Function:
void CSponzaPass::initV()
{
	m_pShader = std::make_shared<CShader>("Sponza_VS.glsl", "Sponza_FS.glsl");
	ElayGraphics::STexture Texture2D4Mat, Texture2D4Mag;
	Texture2D4Mat.Type4WrapS = Texture2D4Mat.Type4WrapT = GL_CLAMP_TO_EDGE;
	Texture2D4Mag.Type4WrapS = Texture2D4Mag.Type4WrapT = GL_CLAMP_TO_EDGE;
	Texture2D4Mat.isMipmap = Texture2D4Mag.isMipmap = false;
	m_LTCMatrixTexture = loadTextureFromFile("../Textures/LTCLight/ltc_mat.dds", Texture2D4Mat);
	m_LTCMagnitueTexture = loadTextureFromFile("../Textures/LTCLight/ltc_amp.dds", Texture2D4Mag);
	m_LTC_DisneyDiffuse_MatrixTexture = loadTextureFromFile("../Textures/LTCLight/ltc_DisneyDiffuse_NoPI_N32_mat.dds");
	m_pLightSource = std::dynamic_pointer_cast<CLightSource>(ElayGraphics::ResourceManager::getGameObjectByName("LightSource"));
	m_pSponza = std::dynamic_pointer_cast<CSponza>(ElayGraphics::ResourceManager::getGameObjectByName("Sponza"));

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_SponzaModelMatrix));
	m_pShader->setTextureUniformValue("u_LTC_MatrixTexture", m_LTCMatrixTexture);
	m_pShader->setTextureUniformValue("u_LTC_MagnitueTexture", m_LTCMagnitueTexture);
	m_pShader->setTextureUniformValue("u_LTC_DisneyDiffuse_MatrixTexture", m_LTC_DisneyDiffuse_MatrixTexture);
	m_pSponza->initModel(*m_pShader);

	m_pLightInfo = ElayGraphics::ResourceManager::getSharedDataByName<SLight*>("LightInfo");
	m_LightInfoByteSize = ElayGraphics::ResourceManager::getSharedDataByName<size_t>("LightInfoByteSize");
	m_LightInfoSSBO = genBuffer(GL_SHADER_STORAGE_BUFFER, m_LightInfoByteSize, m_pLightInfo, GL_STATIC_DRAW, 1);
	auto LightNum = ElayGraphics::ResourceManager::getSharedDataByName<size_t>("LightNum");
	m_pShader->setIntUniformValue("u_LightNum", LightNum);

	__initRoamingPathFromFile("CameraPos.txt");
}

//************************************************************************************
//Function:
void CSponzaPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_MULTISAMPLE);

	transferData2Buffer(GL_SHADER_STORAGE_BUFFER, m_LightInfoSSBO, { 0 }, { static_cast<int>(m_LightInfoByteSize) }, { m_pLightInfo });

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

	const glm::mat4& SponzaModelMatrix = m_pSponza->getModelMatrix();
	if (SponzaModelMatrix != m_SponzaModelMatrix)
	{
		m_SponzaModelMatrix = SponzaModelMatrix;
		m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_SponzaModelMatrix));
	}

	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_PRESS && m_OldKeyPStatus != GLFW_PRESS)
	{
		m_OldKeyPStatus = GLFW_PRESS;
		m_EnableLTC = !m_EnableLTC;
		m_pShader->setIntUniformValue("u_EnableLTC", m_EnableLTC);
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_RELEASE)
		m_OldKeyPStatus = GLFW_RELEASE;

	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_K) == GLFW_PRESS && m_OldKeyKStatus != GLFW_PRESS)
	{
		m_OldKeyKStatus = GLFW_PRESS;
		m_EnableKeyK = !m_EnableKeyK;
		m_pShader->setIntUniformValue("u_EnableKeyK", m_EnableKeyK);
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_K) == GLFW_RELEASE)
		m_OldKeyKStatus = GLFW_RELEASE;
	
	ElayGraphics::Camera::setMainCameraPos(m_CameraPosSet[m_CurrentFrame]);
	m_pShader->setFloatUniformValue("u_CameraPosInWorldSpace", m_CameraPosSet[m_CurrentFrame].x, m_CameraPosSet[m_CurrentFrame].y, m_CameraPosSet[m_CurrentFrame].z);
	if (m_CurrentFrame >= m_CameraMoveSteps - 1) m_CurrentFrame = 0;
	else m_CurrentFrame++;

	m_pSponza->updateModel(*m_pShader);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//************************************************************************************
//Function:
void CSponzaPass::__initRoamingPathFromFile(const std::string& vFileName)
{
	std::ifstream CameraFileIn(vFileName, std::ios::in);
	if (!CameraFileIn)
		std::cout << "Error : Failed open file." << std::endl;
	else
	{
		std::string str;
		getline(CameraFileIn, str);
		float a = sizeof(str);
		float b = sizeof(' ');
		float c = sizeof(glm::vec3);
		CameraFileIn.seekg(0, std::ios::end);
		const int FileLength = CameraFileIn.tellg();
		CameraFileIn.seekg(0, std::ios::beg);
		const int CameraPosNum = FileLength / (sizeof(glm::vec3) + 4 * sizeof(char));
		m_CameraMoveSteps = CameraPosNum;
		m_CameraPosSet.reserve(CameraPosNum);
		float TempX, TempY, TempZ;
		for (int i = 0; i < CameraPosNum; ++i)
		{

			CameraFileIn >> TempX;
			CameraFileIn >> TempY;
			CameraFileIn >> TempZ;
			m_CameraPosSet.emplace_back(TempX, TempY, TempZ);
		}
		CameraFileIn.close();
	}
}