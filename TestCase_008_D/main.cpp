#include "Interface.h"
#include "SphereSet.h"
#include "SphereMap2CubeMapPass.h"
#include "ShowCubeMapPass.h"
#include "SpherePBRPass.h"
#include "GaussianBlurPass.h"
#include "BRDFMapPass.h"

int main()
{
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1600, 800);
	//ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CSphereSet>("SphereSet", 1));

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGaussianBlurPass>("GaussianBlurPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSphereMap2CubeMapPass>("SphereMap2CubeMapPass",2));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CBRDFMapPass>("BRDFMapPass", 3));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShowCubeMapPass>("ShowCubeMapPass", 4));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSpherePBRPass>("SpherePBRPass", 5));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}