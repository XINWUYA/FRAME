#include "ResourceManager.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <iostream>
#include "Camera.h"
#include "UBO4ProjectionWorld.h"
#include "Shader.h"
#include "FirstPass.h"
#include "Utils.h"
#include "Model.h"
#include "ScreenQuadPass.h"
#include "LoadModelPass.h"
#include "LoadModelCullFrontPass.h"
#include "RenderPass.h"

CResourceManager::CResourceManager()
{
	__init();
}

CResourceManager::~CResourceManager()
{
}

//************************************************************************************
//Function:
void CResourceManager::__init()
{
	m_pRenderPassSet.reserve(10);
}

//************************************************************************************
//Function: Note:
GLint CResourceManager::getOrCreateScreenQuadVAO()
{
	if (m_ScreenQuadVAO == 0)      //Fixed Me: some method to simply these code
	{
		m_ScreenQuadVAO = createVAO4ScreenQuad();
	}
	return m_ScreenQuadVAO;
}

//************************************************************************************
//Function:
const std::shared_ptr<const CModel> CResourceManager::getOrCreateModel(const std::string &vModelPath)
{
	std::vector<std::string> SplitStringSet;
	boost::split(SplitStringSet, vModelPath, boost::is_any_of("/\\"));
	std::string ModelName = *(SplitStringSet.end() - 1);
	if (m_pModelSet.find(ModelName) == m_pModelSet.end())
		m_pModelSet[ModelName] = std::make_shared<CModel>(vModelPath);
	return m_pModelSet[ModelName];
}

//************************************************************************************
//Function:
std::shared_ptr<CUBO4ProjectionWorld> CResourceManager::fetchOrCreateUBO4ProjectionWorld()
{
	if(!m_pUBO4ProjectionWorld)
		m_pUBO4ProjectionWorld= std::make_shared<CUBO4ProjectionWorld>();
	return m_pUBO4ProjectionWorld; 
}

//************************************************************************************
//Function:
std::shared_ptr<CCamera> CResourceManager::fecthOrCreateMainCamera()
{
	if(!m_pMainCamera)
		m_pMainCamera = std::make_shared<CCamera>();
	return m_pMainCamera; 
}

//************************************************************************************
//Function:
void CResourceManager::registerRenderPass(const std::shared_ptr<IRenderPass> &vRenderPass)
{
	_ASSERT(vRenderPass);
	//m_pRenderPassSet.insert(std::lower_bound(m_pRenderPassSet.begin(), m_pRenderPassSet.end(), vRenderPass, [](const std::shared_ptr<IRenderPass> &vPass1, const std::shared_ptr<IRenderPass> &vPass2) {return *vPass1 < *vPass2; }), vRenderPass);   //sort when insert
	m_pRenderPassSet.push_back(vRenderPass);
}

//************************************************************************************
//Function:
const std::shared_ptr<IRenderPass>& CResourceManager::getRenderPassByName(const std::string &vPassName) const
{
	for (const auto &vItem : m_pRenderPassSet)
	{
		if (vPassName == vItem->getPassName())
			return vItem;
	}
	return nullptr;
}

//************************************************************************************
//Function:
void CResourceManager::registerTexture2D(const std::shared_ptr<ElayGraphics::STexture2D>& vTexture2D)
{
	if (vTexture2D->TextureName.empty())
	{
		std::cerr << "Please set texture name." << std::endl;
		return;
	}
	m_Texture2DSet.insert({ vTexture2D->TextureName, vTexture2D });
}

//************************************************************************************
//Function:
const std::shared_ptr<ElayGraphics::STexture2D>& CResourceManager::getTexture2DByName(const std::string &vTextureName)
{
	if (m_Texture2DSet.find(vTextureName) != m_Texture2DSet.end())
		return m_Texture2DSet[vTextureName];
	else
	{
		std::cerr << "Texture call " + vTextureName + " is not in Resource Manager." << std::endl;
		return nullptr;
	}
}

//************************************************************************************
//Function:
void CResourceManager::sortRenderPassSetByExecutionOrder()
{
	std::sort(m_pRenderPassSet.begin(), m_pRenderPassSet.end());
}