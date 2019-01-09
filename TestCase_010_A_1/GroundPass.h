#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>

class CLightSource;
class CGround;

class CGroundPass : public IRenderPass
{
public:
	CGroundPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CGroundPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	float  m_Roughness = 0.0f;
	float  m_Metalness = 0.0f;
	float  m_Intensity = 0.0f;
	bool   m_EnableLTC = true;
	int m_LTCMatrixTexture = -1;
	int m_LTCMagnitueTexture = -1;
	int m_LTC_DisneyDiffuse_MatrixTexture = -1;
	int m_RoughnessTexture = -1;
	int m_OldKeyPStatus = -1;
	glm::vec4 m_Albedo;
	glm::vec3 m_LightSourcePos;
	std::shared_ptr<CLightSource> m_pLightSource;
	std::shared_ptr<CGround> m_pGround;
	glm::mat4 m_GroundModelMatrix;
};