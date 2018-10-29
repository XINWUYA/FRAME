#pragma once
#include "RenderPass.h"
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>

class CGroundPass : public IRenderPass
{
public:
	CGroundPass(const std::string& vPassName, int vExcutionOrder);

	virtual void initV() override;
	virtual void updateV() override;

private:
	GLuint m_LTCMatrixTexture = 0;
	GLuint m_LTCMagnitueTexture = 0;
	glm::vec4 m_DiffuseColor;
	glm::vec4 m_SpecularColor;
	std::vector<glm::vec3> m_PolygonalLightVertexPos;
	float  m_Roughness = 0.0f;
	float  m_Intensity = 0.0f;
	bool   m_IsTwoSide = false;
};