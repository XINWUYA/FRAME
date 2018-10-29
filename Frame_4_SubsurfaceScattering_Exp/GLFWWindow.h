#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Singleton.h"

class CGLFWWindow : public CSingleton<CGLFWWindow>
{
	friend class CSingleton<CGLFWWindow>;				//In order to access this private constructor when new object in CSingleton
public:
	~CGLFWWindow();
	void init();
	GLFWwindow* fetchWindow() const;
	
private:
	CGLFWWindow();
	void __setViewport();

	GLFWwindow *m_pWindow = nullptr;					//If set m_pWindow as shared_ptr, will result in warning: delete incomplete type pointer, no call destructor
};