#include "Interface.h"
#include "SponzaPass.h"
#include "MyGUI.h"
#include "Sponza.h"
#include "LightSource.h"
#include "LightSourcePass.h"

int main()
{
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSponza>("Sponza", 1));
	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CLightSource>("LightSource", 2));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSponzaPass>("SponzaPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CLightSourcePass>("LightSourcePass", 2));

	ElayGraphics::ResourceManager::registerSubGUI(std::make_shared<CMyGUI>("MyGUI", 1));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}