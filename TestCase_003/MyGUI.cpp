#include "MyGUI.h"
#include <iostream>
#include <sstream>
#include <iconfontheaders/icons_font_awesome.h>
#include "Common.h"
#include "Interface.h"

CMyGUI::CMyGUI(const std::string& vName, int vExcutionOrder) : IGUI(vName, vExcutionOrder)
{
}

CMyGUI::~CMyGUI()
{
}

//************************************************************************************
//Function:
void CMyGUI::initV()
{
	//---------------------------------��������------------------------------------
	loadFont("../Fonts/roboto-regular.ttf", 18);
	loadFont("../Fonts/times.ttf", 16);
}

//************************************************************************************
//Function:
void CMyGUI::updateV()
{
	//---------------------------------������------------------------------------
	if (IGUI::combo("Fonts", m_SelectedID, { "roboto", "Times new roma" }))
	{
		switch (m_SelectedID)
		{
		case 0:
			std::cout << "roboto" << std::endl;
			break;;
		case 1:
			std::cout << "Times new roma" << std::endl;
			break;
		default:
			break;
		}
	}
	//---------------------------------��������------------------------------------
	if (m_SelectedID == 0)
		IGUI::pushFont(IGUI::getFontByName("roboto-regular.ttf"));
	else
		IGUI::pushFont(IGUI::getFontByName("times.ttf"));

	//---------------------------------�˵���------------------------------------
	if (IGUI::beginMenuBar())
	{
		if (IGUI::beginMenu("Menu"))
		{
			if (IGUI::menuItem("Open"))
				std::cout << "Open" << std::endl;
			if (IGUI::menuItem("Close"))
				std::cout << "Close" << std::endl;
			if (IGUI::beginMenu("Add"))
			{
				IGUI::menuItem("Add File");
				IGUI::menuItem("Add Project");
				IGUI::endMenu();
			}
			IGUI::endMenu();
		}
		if (IGUI::beginMenu("Help"))
		{
			//---------------------------------����ɫ���ı�------------------------------------
			IGUI::textColored({ 1,0,1,1 }, "Pink Help");
			IGUI::endMenu();
		}
		IGUI::endMenuBar();
	}

	IGUI::textDisabled("Disabled Text!");

	//---------------------------------��ѡ��------------------------------------
	if (IGUI::checkBox("AnotherWindow", m_IsShowAnotherWindow))
		std::cout << "Check box is clicked." << std::endl;

	//---------------------------------��ֱ���------------------------------------
	IGUI::verticalSpacing();
	IGUI::verticalSpacing();
	IGUI::verticalSpacing();
	IGUI::verticalSpacing();
	IGUI::verticalSpacing();
	IGUI::verticalSpacing();
	IGUI::verticalSpacing();
	IGUI::verticalSpacing();

	//---------------------------------ˮƽ������------------------------------------
	float Min = 0.0f, Max = 1.0f;
	IGUI::sliderScalar("SlideFloat", ElayGraphics::EDataType::DataType_Float, &m_SlideFloat, &Min, &Max);

	//---------------------------------��ֱ������------------------------------------
	int IMin = -10, IMax = 10;
	IGUI::verticalSilderScalar("VSliderInt", { 40,160 }, ElayGraphics::EDataType::DataType_I32, &m_VSliderInt, &IMin, &IMax);

	//---------------------------------ˮƽ�ָ���------------------------------------
	IGUI::horizontalLine();

	//---------------------------------����ɫ�İ�ť------------------------------------
	if (IGUI::colorButton("Change Color Picker Type", { 1,1,0,1 }, { 60,20 }))
		m_IsColorWheelOrBar = !m_IsColorWheelOrBar;

	//---------------------------------��ɫѡ����------------------------------------
	IGUI::setColorPickerType(m_IsColorWheelOrBar);
	IGUI::colorEdit("Color", m_Color);

	//---------------------------------ͬ������------------------------------------
	IGUI::sameLine();
	std::ostringstream Outs;
	Outs << "Color is " << m_Color[0] << "," << m_Color[1] << "," << m_Color[2] << "," << m_Color[3];
	IGUI::text(Outs.str());

	//---------------------------------��ť���ַ�ͼ�꣩------------------------------------
	if (IGUI::button(ICON_FA_REPEAT" Button"))
		++m_Counter;
	IGUI::sameLine();
	IGUI::text(std::to_string(m_Counter));

	//---------------------------------�۵���------------------------------------
	if (IGUI::collapsingHeader("Introduction"))
	{
		if (IGUI::treeNode("1.1 Origin"))
		{
			//---------------------------------��������------------------------------------
			IGUI::bulletText("Origin Time");
			IGUI::bullet();
			IGUI::button("Button1");
			IGUI::sameLine();
			IGUI::promptMarker("(?)", "This story origin from 1984...");
			if (IGUI::treeNode("1.1.1 Origin place"))
			{
				IGUI::text("British");
				//---------------------------------����------------------------------------
				IGUI::indent();
				IGUI::text("American");
				//---------------------------------������------------------------------------
				IGUI::unIndent();
				IGUI::text("Roma");
				IGUI::treePop();
			}
			IGUI::treePop();
		}
	}

	//---------------------------------��ѡ��ť------------------------------------
	IGUI::radioButton("1973", m_Year, 0);
	IGUI::sameLine();
	IGUI::radioButton("1984", m_Year, 1);
	IGUI::sameLine();
	IGUI::radioButton("1995", m_Year, 2);

	//---------------------------------����------------------------------------
	std::vector<float> DataSet{ 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
	IGUI::plotLines("Curve", DataSet);

	//---------------------------------����ͼ------------------------------------
	IGUI::plotHistogram("Histogram", DataSet, 0, 1, { 0,80 });

	//------------------------�����߻�����ͼ�����ƺ�����֡�ʺܵͣ�--------------------
	auto Func = [](void*, int i) {return sinf(i*0.1f); };
	int MinSampleNum = 1, MaxSampleNum = 400;
	IGUI::sliderScalar("Sample Count", ElayGraphics::EDataType::DataType_I32, &m_SampleNum, &MinSampleNum, &MaxSampleNum);
	IGUI::plotLines("Sin Func Curve", Func, m_SampleNum, -1, 1, { 0,80 });
	IGUI::plotHistogram("Sin Func Histogram", Func, m_SampleNum, -1, 1, { 0,80 });

	//---------------------------------������------------------------------------
	m_Progress += m_ProgressDir * 0.4 * ElayGraphics::App::getDeltaTime();
	if (m_Progress >= 1.0 || m_Progress <= 0.0) m_ProgressDir = -m_ProgressDir;
	IGUI::processBar(m_Progress, { 0,0 });

	//---------------------------------������һ��UI�Ŀ��------------------------
	IGUI::pushItemWidth(300);
	//---------------------------------�����------------------------------------	FIXME: û���˸�
	IGUI::inputText("Input Text", m_InputStr);
	IGUI::popItemWidth();
	IGUI::sameLine();
	IGUI::text(m_InputStr);
	IGUI::inputText("Password", m_InputPassword, 128, true);
	IGUI::inputInt("Input Int", m_InputInt);
	IGUI::sameLine();
	IGUI::text(std::to_string(m_InputInt));
	IGUI::inputFloat("Input Float", m_InputFloat);
	IGUI::sameLine();
	IGUI::text(std::to_string(m_InputFloat));
	IGUI::inputDouble("Input Double", m_InputDouble);
	IGUI::sameLine();
	IGUI::text(std::to_string(m_InputDouble));
	IGUI::inputFloat4("Input Float4", m_InputFloat4);
	IGUI::sameLine();
	IGUI::text(std::to_string(m_InputFloat4[0]) + "," + std::to_string(m_InputFloat4[1]) + "," + std::to_string(m_InputFloat4[2]) + "," + std::to_string(m_InputFloat4[3]) + ",");

	//---------------------------------��ק����------------------------------------
	int DragMinNum = 0, DragMaxNum = 0;
	IGUI::dragScalar("Drag num in -100 100", ElayGraphics::EDataType::DataType_I32, &m_DragNum, 5.0, &DragMinNum, &DragMaxNum);

	//---------------------------------�嵥�б�------------------------------------
	std::vector<std::string> ListBoxItems{ "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
	IGUI::listBox("ListBox", m_CurrentItemInListBox, ListBoxItems, 3);

	if (m_IsShowAnotherWindow)
	{
		IGUI::begin("Another Window", m_IsShowAnotherWindow);
		//---------------------------------�Ҽ�������------------------------------------
		if (IGUI::beginPopupContextItem())
		{
			if (IGUI::menuItem("Close"))
				m_IsShowAnotherWindow = false;
			IGUI::endPopup();
		}
		if (IGUI::button("Popup"))
			IGUI::openPopup("TestPopup");
		if (IGUI::beginPopup("TestPopup"))
		{
			IGUI::text("PopupText");
			IGUI::endPopup();
		}
		if (IGUI::button("Delete.."))
			IGUI::openPopup("Delete?");
		if (IGUI::beginPopupModal("Delete?"))
		{
			if (IGUI::button("Close"))
				IGUI::closeCurrentPopup();
			IGUI::endPopup();
		}

		IGUI::text("This is another window.");
		//---------------------------------ΪItem������ɫ------------------------------------
		IGUI::pushStyleColor(ElayGraphics::EGUIItemColor::GUIItemColor_Button, { 0.3,0.4,0.4 ,1.0 });
		IGUI::pushStyleColor(ElayGraphics::EGUIItemColor::GUIItemColor_ButtonHovered, { 0.4, 0.5, 0.5 ,1.0 });
		IGUI::pushStyleColor(ElayGraphics::EGUIItemColor::GUIItemColor_ButtonActive, { 0.5, 0.6, 0.6,1.0 });
		if (IGUI::button("Close me"))
			m_IsShowAnotherWindow = false;
		IGUI::popStyleColor(3);
		IGUI::end();
	}

	IGUI::popFont();
}