#include "Bunny.h"
#include "Interface.h"

CBunny::CBunny(const std::string& vName, int vExecutionOrder) : IGameObject(vName, vExecutionOrder)
{
}

//************************************************************************************
//Function:
void CBunny::initV()
{
	setModel(ElayGraphics::ResourceManager::getOrCreateModel("../../Model/Bunny/bunny.obj"));
}

//************************************************************************************
//Function:
void CBunny::updateV()
{
	setRotationY(0.1);
}