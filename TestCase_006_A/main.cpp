#include "Interface.h"
#include "ShowSphereMapPass.h"

int main()
{
	//ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShowSphereMapPass>("ShowSphereMapPass", 1));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}