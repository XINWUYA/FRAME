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
	setModel(ElayGraphics::ResourceManager::getOrCreateModel("../Model/CornellBox/CornellBox2/CornellBox2.obj"));
}

void CCornelBox::updateV()
{
}