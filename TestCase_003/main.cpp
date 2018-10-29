#include <memory>
#include "Common.h"
#include "MyGUI.h"
#include "Interface.h"

int main()
{
	{
		ElayGraphics::WINDOW_KEYWORD::setIsCursorDisable(false);
		//ElayGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

		ElayGraphics::ResourceManager::registerSubGUI(std::make_shared<CMyGUI>("MyGUI", 1));

		ElayGraphics::App::initApp();
		ElayGraphics::App::updateApp();
	}

	return 0;
}