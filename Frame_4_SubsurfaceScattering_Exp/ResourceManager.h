//////////////////////////////////////////////////////////////////////////
//The resource manager is mainly used to store the global resources that will be shared in multiple pass
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <GL/glew.h>
#include <set>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "common.h"
#include "Singleton.h"

class CCamera;
class CUBO4ProjectionWorld;
class CShader;
class CFirstPass;
class CModel;
class CScreenQuadPass;
class CLoadModelPass;
class CLoadModelCullFrontPass;
class IRenderPass;

class CResourceManager : public CSingleton<CResourceManager>
{
	friend class CSingleton<CResourceManager>;
public:
	~CResourceManager();

	GLint											 getOrCreateScreenQuadVAO();
	const std::shared_ptr<const CModel>				 getOrCreateModel(const std::string &vModelPath);
	const std::shared_ptr<IRenderPass>&				 getRenderPassByName(const std::string &vPassName) const;
	const std::vector<std::shared_ptr<IRenderPass>>& getRenderPassSet() const { return m_pRenderPassSet; }
	const std::shared_ptr<ElayGraphics::STexture2D>&		 getTexture2DByName(const std::string &vTextureName);

	std::shared_ptr<CCamera>              fecthOrCreateMainCamera();
	std::shared_ptr<CUBO4ProjectionWorld> fetchOrCreateUBO4ProjectionWorld();

	void registerRenderPass(const std::shared_ptr<IRenderPass> &vRenderPass);
	void registerTexture2D(const std::shared_ptr<ElayGraphics::STexture2D>& vTexture2D);

	void sortRenderPassSetByExecutionOrder();

private:
	CResourceManager();
	void __init();

	std::shared_ptr<CUBO4ProjectionWorld>   m_pUBO4ProjectionWorld;
	std::shared_ptr<CCamera>                m_pMainCamera;

	GLint m_ScreenQuadVAO = 0;
	GLint m_Texture4FisrtPass = 0;
	GLint m_Texture4LoadModelCullFrontPass = 0;
	std::vector<std::shared_ptr<IRenderPass>> m_pRenderPassSet;
	std::map<std::string, std::shared_ptr<CModel>> m_pModelSet;
	std::map<std::string, std::shared_ptr<ElayGraphics::STexture2D>> m_Texture2DSet;
};