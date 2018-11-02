#include "Interface.h"
#include "SphereMap2CubeMapPass.h"
#include "ShowCubeMapPass.h"

int main()
{
	//ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSphereMap2CubeMapPass>("SphereMap2CubeMap", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShowCubeMapPass>("ShowCubeMap", 2));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}