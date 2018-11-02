#include "Interface.h"
#include "SphereMap2CubeMapPass.h"
#include "ConvolutionEnvCubeMapPass.h"
#include "ShowCubeMapPass.h"

int main()
{
	//ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSphereMap2CubeMapPass>("SphereMap2CubeMapPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CConvolutionEnvCubeMapPass>("ConvolutionEnvCubeMapPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShowCubeMapPass>("ShowCubeMapPass", 3));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();
}