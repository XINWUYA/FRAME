#include "MyGUI.h"
#include "Interface.h"
#include "Common.h"

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
	ElayGraphics::ResourceManager::registerSharedData("Albedo", m_Albedo);
	ElayGraphics::ResourceManager::registerSharedData("Roughness", m_Roughness);
	ElayGraphics::ResourceManager::registerSharedData("Metalness", m_Metalness);
	ElayGraphics::ResourceManager::registerSharedData("Intensity", m_Intensity);
	ElayGraphics::ResourceManager::registerSharedData("Ground Rotation", m_GroundRotation);
	ElayGraphics::ResourceManager::registerSharedData("Light Position", m_LightSourcePosition);
}

//************************************************************************************
//Function:
void CMyGUI::updateV()
{
	if (IGUI::colorEdit("Albedo", m_Albedo))
		ElayGraphics::ResourceManager::updateSharedDataByName("Albedo", m_Albedo);
	float MinRoughness = 0.0f, MaxRoughness = 1.0f;
	if (IGUI::sliderScalar("Roughness", ElayGraphics::EDataType::DataType_Float, &m_Roughness, &MinRoughness, &MaxRoughness))
		ElayGraphics::ResourceManager::updateSharedDataByName("Roughness", m_Roughness);
	float MinMetalness = 0.0f, MaxMetalness = 1.0f;
	if (IGUI::sliderScalar("Metalness", ElayGraphics::EDataType::DataType_Float, &m_Metalness, &MinMetalness, &MaxMetalness))
		ElayGraphics::ResourceManager::updateSharedDataByName("Metalness", m_Metalness);
	float MinIntensity = 0.0f, MaxIntensity = 10.0f;
	if (IGUI::sliderScalar("Intensity", ElayGraphics::EDataType::DataType_Float, &m_Intensity, &MinIntensity, &MaxIntensity))
		ElayGraphics::ResourceManager::updateSharedDataByName("Intensity", m_Intensity);
	float MinPosOffset = -10.0f, MaxPosOffset = 10.0f;
	if (IGUI::sliderScalar("Light Position X", ElayGraphics::EDataType::DataType_Float, &m_LightSourcePosition.x, &MinPosOffset, &MaxPosOffset))
		ElayGraphics::ResourceManager::updateSharedDataByName("Light Position", m_LightSourcePosition);
	if (IGUI::sliderScalar("Light Position Y", ElayGraphics::EDataType::DataType_Float, &m_LightSourcePosition.y, &MinPosOffset, &MaxPosOffset))
		ElayGraphics::ResourceManager::updateSharedDataByName("Light Position", m_LightSourcePosition);
	if (IGUI::sliderScalar("Light Position Z", ElayGraphics::EDataType::DataType_Float, &m_LightSourcePosition.z, &MinPosOffset, &MaxPosOffset))
		ElayGraphics::ResourceManager::updateSharedDataByName("Light Position", m_LightSourcePosition);
	float MinRotation = 0.0f, MaxRotation = 360.0f;
	if (IGUI::sliderScalar("Ground Rotation X", ElayGraphics::EDataType::DataType_Float, &m_GroundRotation.x, &MinRotation, &MaxRotation))
		ElayGraphics::ResourceManager::updateSharedDataByName("Ground Rotation", m_GroundRotation);
	if (IGUI::sliderScalar("Ground Rotation Y", ElayGraphics::EDataType::DataType_Float, &m_GroundRotation.y, &MinRotation, &MaxRotation))
		ElayGraphics::ResourceManager::updateSharedDataByName("Ground Rotation", m_GroundRotation);
	if (IGUI::sliderScalar("Ground Rotation Z", ElayGraphics::EDataType::DataType_Float, &m_GroundRotation.z, &MinRotation, &MaxRotation))
		ElayGraphics::ResourceManager::updateSharedDataByName("Ground Rotation", m_GroundRotation);
}