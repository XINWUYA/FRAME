#pragma once
#include "GameObject.h"

class CSphere : public IGameObject
{
public:
	CSphere();
	virtual ~CSphere();

	virtual void initV() override;
	virtual void updateV() override;

private:
};