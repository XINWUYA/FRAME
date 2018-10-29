#pragma once
#include "GUI.h"

class CMyGUI : public IGUI
{
public:
	CMyGUI(const std::string& vName, int vExcutionOrder);
	virtual ~CMyGUI();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_SelectedID = -1;
	bool m_IsShowAnotherWindow = false;
	bool m_IsColorWheelOrBar = true;
	float m_SlideFloat = 0.0f;
	float m_Progress = 0.0;
	float m_ProgressDir = 1.0;
	int   m_VSliderInt = 0;
	int	  m_Counter = 0;
	int	  m_Year = 0;
	int	  m_SampleNum = 100;
	int	  m_DragNum = 0;
	int	  m_CurrentItemInListBox = 0;
	int	  m_InputInt = 0;
	float m_InputFloat = 0.0f;
	double m_InputDouble = 0.0;
	glm::vec4	m_InputFloat4;
	std::string m_InputStr;
	std::string m_InputPassword;
	glm::vec4	m_Color;
};