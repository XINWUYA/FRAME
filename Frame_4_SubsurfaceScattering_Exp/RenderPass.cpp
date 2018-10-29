#include "RenderPass.h"

IRenderPass::IRenderPass(const std::string& vPassName, int vExcutionOrder):m_PassName(vPassName),m_ExecutionOrder(vExcutionOrder)
{
}

//************************************************************************************
//Function:
bool IRenderPass::operator<(const IRenderPass& vOtherPass) const
{
	return m_ExecutionOrder < vOtherPass.getExecutionOrder();
}