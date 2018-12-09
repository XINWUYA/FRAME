#pragma once
#include "GameObject.h"

class CCornelBox :public IGameObject
{
public:
	CCornelBox(const std::string& vName, int vExecutionOrder);
	virtual ~CCornelBox();

	virtual void initV() override;
	virtual void updateV() override;

private:
};