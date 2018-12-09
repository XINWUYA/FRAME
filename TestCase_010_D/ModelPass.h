#pragma once
#include "RenderPass.h"
#include <memory>
#include <GLM/glm.hpp>

class IGameObject;
class CLightSource;

class CModelPass : public IRenderPass
{
public:
	CModelPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CModelPass();

	virtual void initV();
	virtual void updateV();

private:
	float  m_Roughness = 0.0f;
	float  m_Metalness = 0.0f;
	float  m_Intensity = 0.0f;
	bool   m_EnableLTC = true;
	int m_LTCMatrixTexture = -1;
	int m_LTCMagnitueTexture = -1;
	int m_OldKeyPStatus = -1;
	glm::vec4 m_Albedo;
	glm::vec3 m_LightSourcePos;
	std::shared_ptr<IGameObject> m_pGameObject;
	std::shared_ptr<CLightSource> m_pLightSource;
	glm::mat4 m_GameObjectModelMatrix;
};