#include "Interface.h"
#include "ScreenQuadPass.h"
#include "GaussianBlurPass.h"

int main()
{
	//ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);
	ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(false);
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1600, 800);

	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGaussianBlurPass>("DrawTextureArrayByFBO", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 2));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();
}