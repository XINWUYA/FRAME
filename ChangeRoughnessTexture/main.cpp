#include "Interface.h"
#include "ChangeRoughness.h"

int main()
{
	ElayGraphics::ResourceManager::registerRenderPass(std::make_shared<CChangeRoughnessPass>("ChangeRoughness", 1));

	ElayGraphics::App::initApp();
	ElayGraphics::App::updateApp();

	return 0;
}