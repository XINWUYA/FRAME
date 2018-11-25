#include "Interface.h"
#include "SixFaceMap2CubeMapPass.h"
#include "ShowCubeMapPass.h"

int main()
{
	//ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	//ElayGraphics::WINDOW_KEYWORD::setWindowSize(256, 256);
	//ElayGraphics::Camera::setMainCameraFov(90.0);
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CSixFaceMap2CubeMapPass>("SphereMap2CubeMap", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShowCubeMapPass>("ShowCubeMap", 2));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}