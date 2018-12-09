#include "Interface.h"
#include "LightSourcePass.h"
#include "ModelPass.h"
#include "MyGUI.h"
#include "Ground.h"
#include "LightSource.h"
#include "CornelBox.h"

int main()
{
	//ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CCornelBox>("CornelBox", 1));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CLightSource>("LightSource", 2));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CModelPass>("ModelPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CLightSourcePass>("LightSourcePass", 2));

	ElayGraphics::ResourceManager::registerSubGUI(std::make_shared<CMyGUI>("MyGUI", 1));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}