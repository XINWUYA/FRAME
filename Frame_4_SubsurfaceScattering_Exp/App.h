#pragma once
#include <memory>
#include "GLFWWindow.h"
#include "Singleton.h"

class CShader;
class CCamera;
class CResourceManager;

class CApp : public CSingleton<CApp>
{
	friend class CSingleton<CApp>;
public:
	~CApp();

	GLvoid init();
	GLvoid update();
	GLdouble getDeltaTime();

private:
	CApp();
	GLvoid __calculateTime();

	GLFWwindow  *m_pWindow;
	GLdouble     m_DeltaTime = 0.0;
	GLdouble     m_LastFrameTime = 0.0;
	std::shared_ptr<CResourceManager> m_pResourceManager = nullptr;
};