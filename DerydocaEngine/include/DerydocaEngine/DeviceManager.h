#pragma once

#if USE_DX12
#include <DXGI.h>
#include <d3d12.h>
#endif

#define GLFW_INCLUDE_NONE // Do not include any OpenGL headers
#include <GLFW/glfw3.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif // _WIN32
#include <GLFW/glfw3native.h>
#include <nvrhi/nvrhi.h>

namespace DerydocaEngine
{
	struct DeviceManagerSettings
	{
		bool StartFullscreen = false;
		bool AllowModeSwitch = true;
		uint32_t BackbufferWidth = 1280;
		uint32_t BackbufferHeight = 720;
		uint32_t SwapChainSampleCount = 1;
		uint32_t SwapChainSampleQuality = 0;
		uint32_t RefreshRate = 0;
		uint32_t SwapChainBufferCount = 3;
		int WindowPosX = -1;
		int WindowPosY = -1;
		bool EnablePerMonitorDPI = false;
		bool EnableCopyQueue = false;
		bool EnableDebugRuntime = false;
		bool EnableNvrhiValidationLayer = false;
		bool EnableValidationLayer = false;
		bool VSyncEnabled = true;
		nvrhi::Format SwapChainFormat = nvrhi::Format::SRGBA8_UNORM;

#if USE_DX12
		IDXGIAdapter* Adapter = nullptr;
		DXGI_USAGE SwapChainUsage = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT;
		D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_1;
#endif

		std::wstring AdapterNameSubstring = L"";
	};

	class DeviceManager
	{
	public:
		static DeviceManager* Create(nvrhi::GraphicsAPI gapi);

	public:
		DeviceManager();

		[[nodiscard]] virtual nvrhi::IDevice* GetDevice() const = 0;

		int CreateWindowAndSwapChain(DeviceManagerSettings settings, const char* windowTitle);
		int RunUpdateLoop();
		void Cleanup();

	protected:
		virtual bool CreateDeviceAndSwapChain() = 0;
		virtual void DestroyDeviceAndSwapChain() = 0;
		virtual void Present() = 0;

	private:
		void ProcessWindowEvents();

	protected:
		bool m_WindowVisible = false;
		bool m_Running;
		bool m_RequestedVSync;
		GLFWwindow* m_Window;
		DeviceManagerSettings m_WindowParams;
		const char* m_WindowTitle;

	};
}