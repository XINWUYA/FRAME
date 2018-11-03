#include "Interface.h"
#include "SphereSet.h"
#include "SphereMap2CubeMapPass.h"
#include "ConvolutionEnvCubeMapPass.h"
#include "PrefilterEnvMapPass.h"
#include "BRDFMapPass.h"
#include "ShowCubeMapPass.h"
#include "SpherePBRPass.h"

int main()
{
	//ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSphereSet>("SphereSet", 1));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSphereMap2CubeMapPass>("SphereMap2CubeMapPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CConvolutionEnvCubeMapPass>("ConvolutionEnvCubeMapPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CPrefilterEnvMapPass>("PrefilterEnvMapPass", 3));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CBRDFMapPass>("BRDFMapPass", 4));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShowCubeMapPass>("ShowCubeMapPass", 5));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSpherePBRPass>("ConvolutionEnvCubeMapPass", 6));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}