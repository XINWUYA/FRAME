#pragma once
#include "GUI.h"
#include <GLM/glm.hpp>

class CMyGUI : public IGUI
{
public:
	CMyGUI(const std::string& vName, int vExcutionOrder);
	virtual ~CMyGUI();

	virtual void initV() override;
	virtual void updateV() override;

private:
	glm::vec4 m_DiffuseColor = glm::vec4(1);
	glm::vec4 m_SpecularColor = glm::vec4(1);
	glm::vec3 m_LightSourceRotation;
	glm::vec3 m_LightSourcePosition{ 0,1,0 };
	float	  m_Roughness = 0.1287f;
	float	  m_Metalness = 0.0f;
	float	  m_Intensity = 1.0f;
};