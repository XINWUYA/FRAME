#include "Interface.h"
#include "SphereMap2CubeMapPass.h"
#include "PrefilterEnvMapPass.h"
#include "ShowCubeMapPass.h"

int main()
{
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSphereMap2CubeMapPass>("SpherMap2CubeMapPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CPrefilterEnvMapPass>("PrefilterEnvMapPass", 2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShowCubeMapPass>("ShowCubeMapPass", 3));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}