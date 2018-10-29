#include "ScreenQuad.h"
#include "Interface.h"

CScreenQuad::CScreenQuad(const std::string &vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

//************************************************************************************
//Function:
void CScreenQuad::initV()
{
	setVAO(ElayGraphics::ResourceManager::getOrCreateScreenQuadVAO());
}

//************************************************************************************
//Function:
void CScreenQuad::updateV()
{

}