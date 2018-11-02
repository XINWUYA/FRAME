#include "Interface.h"
#include "BRDFMapPass.h"
#include "Show2DTexturePass.h"

int main()
{
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CBRDFMapPass>("BRDFMapPass", 1));
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CShow2DTexturePass>("Show2DTexturePass", 2));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}