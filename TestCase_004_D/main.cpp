#include "Interface.h"
#include "GaussianBlurPass.h"
#include "ScreenQuadPass.h"

int main()
{
	ElayGraphics::WINDOW_KEYWORD::setWindowSize(1024, 1024);
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CGaussianBlurPass>("GaussianBlurPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 2));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}