#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>
#include <fstream>
#include <vector>

class CLightSource;
class CSponza;

class CSponzaPass : public IRenderPass
{
public:
	CSponzaPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CSponzaPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	float  m_Roughness = 0.0f;
	float  m_Metalness = 0.0f;
	float  m_Intensity = 0.0f;
	bool   m_EnableLTC = true;
	bool   m_EnableKeyK = true;
	int m_LTCMatrixTexture = -1;
	int m_LTCMagnitueTexture = -1;
	int m_LTC_DisneyDiffuse_MatrixTexture = -1;
	int m_LightInfoSSBO = -1;
	int m_OldKeyPStatus = -1;
	int m_OldKeyKStatus = -1;
	int m_CameraMoveSteps = 0;
	int m_CurrentFrame = 0;
	glm::vec4 m_Albedo;
	glm::vec3 m_LightSourcePos;
	std::shared_ptr<CLightSource> m_pLightSource;
	std::shared_ptr<CSponza> m_pSponza;
	glm::mat4 m_SponzaModelMatrix;
	std::ofstream m_CameraPosFileOut;
	std::vector<glm::vec3> m_CameraPosSet;
};