#include "Interface.h"
#include "App.h"
#include "Common.h"
#include "Camera.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "InputManager.h"

//************************************************************************************
//Function:
void ElayGraphics::App::initApp()
{
	CApp::getOrCreateInstance()->init();
}

//************************************************************************************
//Function:
void ElayGraphics::App::updateApp()
{
	CApp::getOrCreateInstance()->update();
}

//************************************************************************************
//Function:
double ElayGraphics::App::getDeltaTime()
{
	return CApp::getOrCreateInstance()->getDeltaTime();
}

//************************************************************************************
//Function:
double ElayGraphics::App::getFrameRateInMilliSecond()
{
	return CApp::getOrCreateInstance()->getFrameRateInMilliSecond();
}

//************************************************************************************
//Function:
int ElayGraphics::App::getFramesPerSecond()
{
	return CApp::getOrCreateInstance()->getFramesPerSecond();
}

//************************************************************************************
//Function:
int ElayGraphics::WINDOW_KEYWORD::getWindowWidth()
{
	return WINDOW_WIDTH;
}

//************************************************************************************
//Function:
int ElayGraphics::WINDOW_KEYWORD::getWindowHeight()
{
	return WINDOW_HEIGHT;
}

//************************************************************************************
//Function:
void ElayGraphics::WINDOW_KEYWORD::setWindowSize(int vWidth, int vHeight, bool vIsViewportSizeChangedWithWindow/* = true*/)
{
	WINDOW_WIDTH = vWidth;
	WINDOW_HEIGHT = vHeight;
	if (vIsViewportSizeChangedWithWindow)
	{
		VIEWPORT_WIDTH = vWidth;
		VIEWPORT_HEIGHT = vHeight;
	}
}

//************************************************************************************
//Function:
void ElayGraphics::WINDOW_KEYWORD::setViewportSize(int vWidth, int vHeight)
{
	VIEWPORT_WIDTH = vWidth;
	VIEWPORT_HEIGHT = vHeight;
}

//************************************************************************************
//Function:
void ElayGraphics::WINDOW_KEYWORD::setSampleNum(int vSampleNum)
{
	NUM_SAMPLES = vSampleNum;
}

//************************************************************************************
//Function:
void ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(bool vIsCursorDisable)
{
	CURSOR_DISABLE = vIsCursorDisable;
}

//************************************************************************************
//Function:
void ElayGraphics::WINDOW_KEYWORD::setWindowTile(const std::string& vWindowTitle)
{
	WINDOW_TITLE = vWindowTitle;
}

//************************************************************************************
//Function:
void ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(bool vIsEnableGUI)
{
	IS_ENABLE_GUI = vIsEnableGUI;
}

//************************************************************************************
//Function:
const glm::dvec3& ElayGraphics::Camera::getMainCameraPos()
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraPos();
}

//************************************************************************************
//Function:
void ElayGraphics::Camera::setMainCameraFarPlane(double vFarPlane)
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->setFarPlane(vFarPlane);
}

//************************************************************************************
//Function:
void ElayGraphics::Camera::setMainCameraMoveSpeed(double vMoveSpeed)
{
	return CResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->setMoveSpeed(vMoveSpeed);
}

//************************************************************************************
//Function:
const std::shared_ptr<IGameObject>& ElayGraphics::ResourceManager::getGameObjectByName(const std::string &vGameObjectName)
{
	return CResourceManager::getOrCreateInstance()->getGameObjectByName(vGameObjectName);
}

//************************************************************************************
//Function:
void ElayGraphics::ResourceManager::registerSharedData(const std::string& vDataName, boost::any vData)
{
	CResourceManager::getOrCreateInstance()->registerSharedData(vDataName, vData);
}

//************************************************************************************
//Function:
void ElayGraphics::ResourceManager::registerRenderPass(const std::shared_ptr<IRenderPass> &vRenderPass)
{
	CResourceManager::getOrCreateInstance()->registerRenderPass(vRenderPass);
}

//************************************************************************************
//Function:
void ElayGraphics::ResourceManager::registerGameObject(const std::shared_ptr<IGameObject> &vGameObject)
{
	CResourceManager::getOrCreateInstance()->registerGameObject(vGameObject);
}

//************************************************************************************
//Function:
void ElayGraphics::ResourceManager::registerSubGUI(const std::shared_ptr<IGUI> &vSubGUI)
{
	CResourceManager::getOrCreateInstance()->registerSubGUI(vSubGUI);
}

//************************************************************************************
//Function:
const std::shared_ptr<CModel>& ElayGraphics::ResourceManager::getOrCreateModel(const std::string &vModelPath) 
{
	return CResourceManager::getOrCreateInstance()->getOrCreateModel(vModelPath);
}

//************************************************************************************
//Function:
GLint ElayGraphics::ResourceManager::getOrCreateScreenQuadVAO()
{
	return CResourceManager::getOrCreateInstance()->getOrCreateScreenQuadVAO();
}

//************************************************************************************
//Function:
GLint ElayGraphics::ResourceManager::getOrCreateCubeVAO()
{
	return CResourceManager::getOrCreateInstance()->getOrCreateCubeVAO();
}

//************************************************************************************
//Function:
const std::shared_ptr<CMainGUI>& ElayGraphics::ResourceManager::getOrCreateMainGUI()
{
	return CResourceManager::getOrCreateInstance()->getOrCreateMainGUI();
}

//************************************************************************************
//Function:
const boost::any& ElayGraphics::ResourceManager::getSharedDataByName(const std::string &vDataName)
{
	return CResourceManager::getOrCreateInstance()->getSharedDataByName(vDataName);
}

//************************************************************************************
//Function:
void ElayGraphics::ResourceManager::updateSharedDataByName(const std::string& vDataName, const boost::any& vData)
{
	CResourceManager::getOrCreateInstance()->updateSharedDataByName(vDataName, vData);
}

//************************************************************************************
//Function:
int ElayGraphics::InputManager::getKeyStatus(int vKey)
{
	return CInputManager::getOrCreateInstance()->getKeyStatus()[vKey];
}