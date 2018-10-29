#include "App.h"
#include <crtdbg.h>
#include "Utils.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Camera.h"
#include "UBO4ProjectionWorld.h"
#include "RenderPass.h"
#include "GLFWWindow.h"
#include "InputManager.h"
#include "GUI.h"
#include "MainGUI.h"
#include "GameObject.h"

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
	CResourceManager::getOrCreateInstance()->fetchOrCreateGLFWWindow()->init();
	m_pWindow = CResourceManager::getOrCreateInstance()->fetchOrCreateGLFWWindow()->fetchWindow();
	m_pResourceManager = CResourceManager::getOrCreateInstance();
	m_pResourceManager->init();
	m_pResourceManager->getOrCreateMainGUI()->init();
	m_pResourceManager->fecthOrCreateMainCamera()->init();
	CInputManager::getOrCreateInstance()->init();
	m_pResourceManager->fetchOrCreateUBO4ProjectionWorld()->init();

	for (auto &vItem : m_pResourceManager->getGameObjectSet())
	{
		vItem->initV();
	}

	for (auto &vItem : m_pResourceManager->getRenderPassSet())
	{
		vItem->initV();
	}

	for (auto &vItem : m_pResourceManager->getSubGUISet())
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

		for (auto &vItem : m_pResourceManager->getGameObjectSet())
		{
			vItem->updateV();
		}

		glClear(GL_COLOR_BUFFER_BIT);
		for (auto &vItem : m_pResourceManager->getRenderPassSet())
		{
			vItem->updateV();
		}

		if (ElayGraphics::COMPONENT_CONFIG::IS_ENABLE_GUI)
		{
			m_pResourceManager->getOrCreateMainGUI()->update();
			for (auto &vItem : m_pResourceManager->getSubGUISet())
			{
				vItem->updateV();
			}
			m_pResourceManager->getOrCreateMainGUI()->lateUpdate();
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

	++m_FrameCounter;
	m_TimeCounter += m_DeltaTime;
	if (m_TimeCounter >= 1.0)
	{
		m_TimeCounter = 0.0;
		m_FramesPerSecond = m_FrameCounter;
		m_FrameCounter = 0;
	}
}

//************************************************************************************
//Function:
GLdouble CApp::getDeltaTime() const
{
	return m_DeltaTime;
}

//************************************************************************************
//Function:
GLdouble CApp::getFrameRateInMilliSecond() const
{
	return m_DeltaTime * 1000;
}

//************************************************************************************
//Function:
GLuint CApp::getFramesPerSecond() const
{
	return m_FramesPerSecond;
}