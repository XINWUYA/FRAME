#pragma once
#include <string>
#include <memory>
#include "Shader.h"
#include "GameObject.h"

class IRenderPass : public CGameObject
{
public:
	IRenderPass() = default;
	IRenderPass(const std::string& vPassName, int vExcutionOrder);

	virtual void initV() = 0;
	virtual void updateV() = 0;

	bool operator<(const IRenderPass& vOtherPass) const;

	const std::string& getPassName() const { return m_PassName; }
	int getExecutionOrder() const { return m_ExecutionOrder; }

	void setPassName(const std::string &vPassName) { m_PassName = vPassName; }
	void setExecutionOrder(int vExecutionOrder) { m_ExecutionOrder = vExecutionOrder; }

protected:
	std::shared_ptr<CShader> m_pShader;

private:
	std::string m_PassName;
	int m_ExecutionOrder = -1;
};