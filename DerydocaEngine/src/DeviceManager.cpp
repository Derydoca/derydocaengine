#include "DerydocaEngine/DeviceManager.h"
#include "DerydocaEngine/Logging/Log.h"

namespace DerydocaEngine
{
	void GLFWErrorCallback(int error, const char* description)
	{
		D_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	static void GLFWCloseCallback(GLFWwindow* window)
	{
		D_LOG_TRACE("Main window close callback was triggered.");
	}

	static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}

	DeviceManager::DeviceManager()
		: m_Running(true)
		, m_Window(NULL)
	{
	}

	int DeviceManager::CreateWindowAndSwapChain(DeviceManagerSettings settings, const char* windowTitle)
	{
		if (!glfwInit())
		{
			D_LOG_ERROR("Unable to init GLFW!");
			return -1;
		}

		glfwSetErrorCallback(GLFWErrorCallback);

		glfwDefaultWindowHints();

		glfwWindowHint(GLFW_SAMPLES, settings.SwapChainSampleCount);
		glfwWindowHint(GLFW_REFRESH_RATE, settings.RefreshRate);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		m_Window = glfwCreateWindow(settings.BackbufferWidth, settings.BackbufferHeight, windowTitle, NULL, NULL);
		if (m_Window)
		{
			D_LOG_ERROR("Window or context creation failed!");
		}

		int fbWidth = 0, fbHeight = 0;
		glfwGetFramebufferSize(m_Window, &fbWidth, &fbHeight);
		m_WindowParams.BackbufferWidth = fbWidth;
		m_WindowParams.BackbufferHeight = fbHeight;

		if (windowTitle)
		{
			m_WindowTitle = windowTitle;
		}

		glfwSetWindowUserPointer(m_Window, this);

		if (settings.WindowPosX != -1 && settings.WindowPosY != -1)
		{
			glfwSetWindowPos(m_Window, settings.WindowPosX, settings.WindowPosY);
		}

		glfwSetWindowCloseCallback(m_Window, GLFWCloseCallback);
		glfwSetKeyCallback(m_Window, GLFWKeyCallback);

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);

		glfwShowWindow(m_Window);

		return 0;
	}

	int DeviceManager::RunUpdateLoop()
	{
		while (!glfwWindowShouldClose(m_Window))
		{
			int width, height;
			float ratio;
			glfwGetFramebufferSize(m_Window, &width, &height);
			ratio = width / (float)height;

			double time = glfwGetTime();

			//glfwSwapBuffers(m_Window);
			glfwPollEvents();
		}
		return 0;
	}

	void DeviceManager::Cleanup()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void DeviceManager::ProcessWindowEvents()
	{
	}
}