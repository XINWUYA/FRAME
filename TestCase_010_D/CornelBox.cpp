#include "CornelBox.h"
#include "Interface.h"

CCornelBox::CCornelBox(const std::string& vName, int vExecutionOrder) : IGameObject(vName, vExecutionOrder)
{
}

CCornelBox::~CCornelBox()
{
}

void CCornelBox::initV()
{
	setModel(ElayGraphics::ResourceManager::getOrCreateModel("../Model/CornellBox/CornellBox2/CornellBox2.obj"));		//../Model/CornellBox/Empty/CornellBox.obj  ../Model/CornellBox/Empty/CornellBox.obj
	//setScale({ 0.1, 0.01, 0.1 });
}

void CCornelBox::updateV()
{
}