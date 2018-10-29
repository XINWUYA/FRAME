#pragma once
#include "GameObject.h"

class CBunny : public IGameObject
{
public:
	CBunny(const std::string& vName, int vExecutionOrder);

	virtual void initV() override;
	virtual void updateV() override;

private:

};