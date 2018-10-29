#include <memory>
#include "App.h"
#include "common.h"
#include "ResourceManager.h"
#include "FirstPass.h"
#include "ScreenQuadPass.h"
#include "LoadModelPass.h"
#include "LoadModelCullFrontPass.h"

int main()
{
	CResourceManager::getOrCreateInstance()->registerRenderPass(std::make_shared<CLoadModelCullFrontPass>("LoadModelCullFrontPass", 1));
	//m_pResourceManager->registerRenderPass(std::make_shared<CScreenQuadPass>("ScreenPass", 3));
	CResourceManager::getOrCreateInstance()->registerRenderPass(std::make_shared<CLoadModelPass>("LoadModelPass", 2));

	std::shared_ptr<CApp> &pApp = CApp::getOrCreateInstance();
	pApp->init();
	pApp->update();

	return 0;
}