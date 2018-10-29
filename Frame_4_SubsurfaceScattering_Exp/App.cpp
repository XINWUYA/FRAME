#include "App.h"
#include <crtdbg.h>
#include "Utils.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Camera.h"
#include "UBO4ProjectionWorld.h"
#include "FirstPass.h"
#include "ScreenQuadPass.h"
#include "LoadModelPass.h"
#include "LoadModelCullFrontPass.h"
#include "GLFWWindow.h"
#include "InputManager.h"
#include <iostream>

std::shared_ptr<CApp> CApp::m_pInstance;

CApp::CApp()
{
}

CApp::~CApp()
{
}

//************************************************************************************
//Function:
GLvoid CApp::init()
{
	CGLFWWindow::getOrCreateInstance()->init();
	m_pWindow = CGLFWWindow::getOrCreateInstance()->fetchWindow();
	CInputManager::getOrCreateInstance()->init();
	m_pResourceManager = CResourceManager::getOrCreateInstance();
	m_pResourceManager->fetchOrCreateUBO4ProjectionWorld()->init();
	m_pResourceManager->sortRenderPassSetByExecutionOrder();
	for (auto &vItem : m_pResourceManager->getRenderPassSet())
	{
		vItem->initV();
	}
}

//************************************************************************************
//Function:
GLvoid CApp::update()
{
	_ASSERT(m_pWindow);
	while (!glfwWindowShouldClose(m_pWindow))
	{
		__calculateTime();
		glfwPollEvents();
		m_pResourceManager->fecthOrCreateMainCamera()->update();
		m_pResourceManager->fetchOrCreateUBO4ProjectionWorld()->update();

		for (auto &vItem : m_pResourceManager->getRenderPassSet())
		{
			vItem->updateV();
		}

		glfwSwapBuffers(m_pWindow);
	}
}

//************************************************************************************
//Function:
GLvoid CApp::__calculateTime()
{
	GLdouble CurrentTime = glfwGetTime();
	m_DeltaTime = CurrentTime - m_LastFrameTime;
	m_LastFrameTime = CurrentTime;
}

//************************************************************************************
//Function:
GLdouble CApp::getDeltaTime()
{
	return m_DeltaTime;
}