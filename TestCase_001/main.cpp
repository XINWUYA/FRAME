#include <memory>
#include "ScreenQuadPass.h"
#include "LoadModelPass.h"
#include "LoadModelCullFrontPass.h"
#include "Bunny.h"
#include "ScreenQuad.h"
#include "Common.h"
#include "Interface.h"

int main()
{
	{
		ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(false);
		ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(true);

		ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CBunny>("Bunny", 1));
		ElayGraphics::ResourceManager::registerGameObject(std::make_shared<CScreenQuad>("ScreenQuad", 1));

		ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CLoadModelCullFrontPass>("LoadModelCullFrontPass", 1));
		//ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenPass", 3));
		ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CLoadModelPass>("LoadModelPass", 2));

		ElayGraphics::App::initApp();
		ElayGraphics::App::updateApp();
	}

	return 0;
}