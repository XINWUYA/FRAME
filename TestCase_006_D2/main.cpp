#include "Interface.h"
#include "SphereMap2CubeMapPass.h"
#include "ConvolutionEnvCubeMapPass.h"
#include "SpherePBRPass.h"
#include "ShowCubeMapPass.h"
#include "SphereSet.h"

int main()
{
	ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSphereSet>("SphereSet", 1));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSphereMap2CubeMapPass>("SphereMap2CubeMapPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CConvolutionEnvCubeMapPass>("ConvolutionEnvCubeMapPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShowCubeMapPass>("ShowCubeMapPass", 3));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSpherePass>("SpherePass", 4));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}