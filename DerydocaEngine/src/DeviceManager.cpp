#include "DerydocaEngine/DeviceManager.h"
#include "DerydocaEngine/Logging/Log.h"
#ifdef USE_DX12
#include "DerydocaEngine/Rendering/DX12/DeviceManagerDX12.h"
#endif
#ifdef USE_VULKAN
#include "DerydocaEngine/Rendering/VK/DeviceManagerVK.h"
#endif
#ifdef PLATFORM_WINDOWS
#include <ShellScalingApi.h>
#pragma comment(lib, "shcore.lib")
#endif

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

	DeviceManager* DeviceManager::Create(nvrhi::GraphicsAPI gapi)
	{
		switch (gapi)
		{
		case nvrhi::GraphicsAPI::D3D11:
			D_LOG_CRITICAL("The graphics API D3D11 is not supported!");
			return nullptr;
#ifdef USE_DX12
		case nvrhi::GraphicsAPI::D3D12:
			return new Rendering::DeviceManagerDX12();
#endif
#ifdef USE_VULKAN
		case nvrhi::GraphicsAPI::VULKAN:
			return new Rendering::DeviceManagerVK();
			return nullptr;
#endif
		default:
			D_LOG_CRITICAL("Invalid graphics API!");
			return nullptr;
		}
	}

	static const struct
	{
		nvrhi::Format format;
		uint32_t redBits;
		uint32_t greenBits;
		uint32_t blueBits;
		uint32_t alphaBits;
		uint32_t depthBits;
		uint32_t stencilBits;
	} formatInfo[] = {
		{ nvrhi::Format::UNKNOWN,            0,  0,  0,  0,  0,  0, },
		{ nvrhi::Format::R8_UINT,            8,  0,  0,  0,  0,  0, },
		{ nvrhi::Format::RG8_UINT,           8,  8,  0,  0,  0,  0, },
		{ nvrhi::Format::RG8_UNORM,          8,  8,  0,  0,  0,  0, },
		{ nvrhi::Format::R16_UINT,          16,  0,  0,  0,  0,  0, },
		{ nvrhi::Format::R16_UNORM,         16,  0,  0,  0,  0,  0, },
		{ nvrhi::Format::R16_FLOAT,         16,  0,  0,  0,  0,  0, },
		{ nvrhi::Format::RGBA8_UNORM,        8,  8,  8,  8,  0,  0, },
		{ nvrhi::Format::RGBA8_SNORM,        8,  8,  8,  8,  0,  0, },
		{ nvrhi::Format::BGRA8_UNORM,        8,  8,  8,  8,  0,  0, },
		{ nvrhi::Format::SRGBA8_UNORM,       8,  8,  8,  8,  0,  0, },
		{ nvrhi::Format::SBGRA8_UNORM,       8,  8,  8,  8,  0,  0, },
		{ nvrhi::Format::R10G10B10A2_UNORM, 10, 10, 10,  2,  0,  0, },
		{ nvrhi::Format::R11G11B10_FLOAT,   11, 11, 10,  0,  0,  0, },
		{ nvrhi::Format::RG16_UINT,         16, 16,  0,  0,  0,  0, },
		{ nvrhi::Format::RG16_FLOAT,        16, 16,  0,  0,  0,  0, },
		{ nvrhi::Format::R32_UINT,          32,  0,  0,  0,  0,  0, },
		{ nvrhi::Format::R32_FLOAT,         32,  0,  0,  0,  0,  0, },
		{ nvrhi::Format::RGBA16_FLOAT,      16, 16, 16, 16,  0,  0, },
		{ nvrhi::Format::RGBA16_UNORM,      16, 16, 16, 16,  0,  0, },
		{ nvrhi::Format::RGBA16_SNORM,      16, 16, 16, 16,  0,  0, },
		{ nvrhi::Format::RG32_UINT,         32, 32,  0,  0,  0,  0, },
		{ nvrhi::Format::RG32_FLOAT,        32, 32,  0,  0,  0,  0, },
		{ nvrhi::Format::RGB32_UINT,        32, 32, 32,  0,  0,  0, },
		{ nvrhi::Format::RGB32_FLOAT,       32, 32, 32,  0,  0,  0, },
		{ nvrhi::Format::RGBA32_UINT,       32, 32, 32, 32,  0,  0, },
		{ nvrhi::Format::RGBA32_FLOAT,      32, 32, 32, 32,  0,  0, },
	};

	int DeviceManager::CreateWindowAndSwapChain(DeviceManagerSettings settings, const char* windowTitle)
	{
#ifdef PLATFORM_WINDOWS
		SetProcessDpiAwareness(settings.EnablePerMonitorDPI ? PROCESS_PER_MONITOR_DPI_AWARE : PROCESS_DPI_UNAWARE);
#endif

		if (!glfwInit())
		{
			D_LOG_ERROR("Unable to init GLFW!");
			return -1;
		}

		m_WindowParams = settings;
		m_RequestedVSync = settings.VSyncEnabled;

		glfwSetErrorCallback(GLFWErrorCallback);

		glfwDefaultWindowHints();

		bool foundFormat = false;
		for (const auto& info : formatInfo)
		{
			if (info.format == settings.SwapChainFormat)
			{
				glfwWindowHint(GLFW_RED_BITS, info.redBits);
				glfwWindowHint(GLFW_GREEN_BITS, info.greenBits);
				glfwWindowHint(GLFW_BLUE_BITS, info.blueBits);
				glfwWindowHint(GLFW_ALPHA_BITS, info.alphaBits);
				glfwWindowHint(GLFW_DEPTH_BITS, info.depthBits);
				glfwWindowHint(GLFW_STENCIL_BITS, info.stencilBits);
				foundFormat = true;
				break;
			}
		}

		assert(foundFormat);

		glfwWindowHint(GLFW_SAMPLES, settings.SwapChainSampleCount);
		glfwWindowHint(GLFW_REFRESH_RATE, settings.RefreshRate);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		m_Window = glfwCreateWindow(settings.BackbufferWidth, settings.BackbufferHeight, windowTitle, NULL, NULL);
		if (!m_Window)
		{
			D_LOG_CRITICAL("Window or context creation failed!");
			return -1;
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

		if (!CreateDeviceAndSwapChain())
		{
			D_LOG_CRITICAL("Unable to create device or swapchain!");
			return -1;
		}

		glfwShowWindow(m_Window);

		return 0;
	}

	int DeviceManager::RunUpdateLoop()
	{
		double previousFrameTime = glfwGetTime();

		while (!glfwWindowShouldClose(m_Window))
		{
			glfwPollEvents();

			double currentFrameTime = glfwGetTime();
			double elapsedFrameTime = currentFrameTime - previousFrameTime;

			ProcessWindowEvents();

			if (m_WindowVisible)
			{
				Present();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(0));

			GetDevice()->runGarbageCollection();
		}

		GetDevice()->waitForIdle();

		return 0;
	}

	void DeviceManager::Cleanup()
	{
		m_SwapChainFramebuffers.clear();

		DestroyDeviceAndSwapChain();

		if (m_Window)
		{
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
		}

		glfwTerminate();
	}

	void DeviceManager::ProcessWindowEvents()
	{
	}
}