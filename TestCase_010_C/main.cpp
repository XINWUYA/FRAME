#include "Interface.h"
#include "GroundPass.h"
#include "MyGUI.h"
#include "Ground.h"
#include "LightSource.h"

int main()
{
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CGround>("Ground", 1));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CLightSource>("LightSource", 2));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGroundPass>("GroundPass", 1));

	ElayGraphics::ResourceManager::registerSubGUI(std::make_shared<CMyGUI>("MyGUI", 1));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}