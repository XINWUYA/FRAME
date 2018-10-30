#include "Interface.h"
#include "ScreenQuadPass.h"
#include "DrawTextureArrayByFBO.h"

int main()
{
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CDrawTextureArrayByFBO>("DrawTextureArrayByFBO", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenQuadPass", 2));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();
}