#pragma once

#include <GLFW/glfw3.h>
#include <nvrhi/nvrhi.h>

namespace DerydocaEngine
{
	struct DeviceManagerSettings
	{
		uint32_t BackbufferWidth = 1280;
		uint32_t BackbufferHeight = 720;
		uint32_t SwapChainSampleCount = 1;
		uint32_t RefreshRate = 0;
		int WindowPosX = -1;
		int WindowPosY = -1;
		bool EnableDebugRuntime = false;
		bool EnableValidationLayer = false;
	};

	class DeviceManager
	{
	public:
		static DeviceManager* Create(nvrhi::GraphicsAPI gapi);

	public:
		DeviceManager();

		int CreateWindowAndSwapChain(DeviceManagerSettings settings, const char* windowTitle);
		int RunUpdateLoop();
		void Cleanup();

	private:
		void ProcessWindowEvents();

	private:
		bool m_Running;
		GLFWwindow* m_Window;
		DeviceManagerSettings m_WindowParams;
		const char* m_WindowTitle;

	};
}