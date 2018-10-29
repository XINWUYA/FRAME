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
	rotate(glm::vec3(0, 0.1, 0));
}