#pragma once
#include "GameObject.h"

class CScreenQuad : public IGameObject
{
public:
	CScreenQuad(const std::string &vGameObjectName, int vExecutionOrder);

	virtual void initV() override;
	virtual void updateV() override;

private:

};