#pragma once

#include <list>
#include <GLFW/glfw3.h>
#include <nvrhi/nvrhi.h>
#ifdef USE_DX12
#include <dxgi.h>
#include <d3d12.h>
#endif
#ifdef D_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include "Derydoca/Rendering/Common.h"
#include "Derydoca/Rendering/CommandBuffer.h"

namespace Derydoca::Rendering
{
	class IApplicationLayer;

	struct DeviceCreationParams
	{
		uint32_t width = 1920;
		uint32_t height = 1080;
		uint32_t sampleCount = 1;
		uint32_t sampleQuality = 0;
		uint32_t windowPosX = -1;
		uint32_t windowPosY = -1;
        ImageFormat imageFormat = ImageFormat::RGBA8_UNORM; // REMOVE
		nvrhi::Format swapChainFormat = nvrhi::Format::SRGBA8_UNORM;
		std::wstring adapterNameSubstring = L"";
		uint32_t bufferCount = 3;
		bool allowModeSwitch = true;
		bool enableDebugRuntime = true;
		bool enableComputeQueue = true;
		bool enableCopyQueue = true;
		uint32_t refreshRate = 0;
		bool startFullscreen = false;
		bool startMaximized = false;
		bool enableNvrhiValidationLayer = true;
		bool vsyncEnabled = false;
		bool enablePerMonitorDPI = false;

#ifdef USE_DX12
		DXGI_USAGE swapChainUsage = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT;
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_0;
#endif
	};

	struct DefaultMessageCallback : public nvrhi::IMessageCallback
	{
		static DefaultMessageCallback& GetInstance();

		void message(nvrhi::MessageSeverity severity, const char* messageText) override;
	};

	class DeviceManager
	{
	public:
#define DIRECT_ENUM_TRANSLATE_FUNC_TO(GenericType, SpecificType) inline SpecificType Translate(GenericType value) { return static_cast<SpecificType>(value); }
#define DIRECT_ENUM_TRANSLATE_FUNC_FROM(GenericType, SpecificType) inline GenericType Translate(SpecificType value) { return static_cast<GenericType>(value); }
#define DIRECT_ENUM_TRANSLATE_FUNCS(GenericType, SpecificType) DIRECT_ENUM_TRANSLATE_FUNC_TO(GenericType, SpecificType); DIRECT_ENUM_TRANSLATE_FUNC_FROM(GenericType, SpecificType)

		static DeviceManager* Create(const RenderingAPI renderingAPI);
		virtual ~DeviceManager() = default;

        virtual void Render() = 0;
		virtual void CreateRenderPass(const RenderPassDesc& renderPassDesc, RenderPass* renderPass) = 0;
		virtual void CreateCommandBuffer(CommandBuffer* commandBuffer) const = 0;

		[[nodiscard]] virtual nvrhi::IDevice* GetDevice();
		[[nodiscard]] uint32_t GetFrameIndex() const { return m_FrameIndex; }
		void Shutdown();
        void SignalWindowResizedEvent();
		bool CreateWindowDeviceAndSwapChain(const DeviceCreationParams& params, const char* windowTitle);

		void RunMessageLoop();

		// these are public in order to be called from the GLFW callback functions
		void WindowCloseCallback() { }
		void WindowIconifyCallback(int iconified) { }
		void WindowFocusCallback(int focused) { }
		void WindowRefreshCallback() { }
		void WindowPosCallback(int xpos, int ypos);

		void KeyboardUpdate(int key, int scancode, int action, int mods);
		void KeyboardCharInput(unsigned int unicode, int mods);
		void MousePosUpdate(double xpos, double ypos);
		void MouseButtonUpdate(int button, int action, int mods);
		void MouseScrollUpdate(double xoffset, double yoffset);

    protected:
		DeviceManager();

		void UpdateWindowSize();
		void BackBufferResizing();
		void BackBufferResized();

		virtual bool CreateDeviceAndSwapChain() = 0;
		virtual void DestroyDeviceAndSwapChain() = 0;
		virtual nvrhi::GraphicsAPI GetGraphicsAPI() const = 0;
		virtual uint32_t GetBackBufferCount() = 0;
		virtual void ResizeSwapChain() = 0;
		virtual nvrhi::ITexture* GetBackBuffer(uint32_t index) = 0;

		// timestamp in seconds for the previous frame
		double m_PreviousFrameTimestamp = 0.0;
		DeviceCreationParams m_DeviceParams;
		GLFWwindow* m_Window = nullptr;
        bool framebufferResized = false;
		bool m_windowVisible = false;
		bool m_RequestedVSync = false;
		// current DPI scale info (updated when window moves)
		float m_DPIScaleFactorX = 1.f;
		float m_DPIScaleFactorY = 1.f;

		std::string m_WindowTitle;

		nvrhi::DeviceHandle m_NvrhiDevice;
		std::vector<nvrhi::FramebufferHandle> m_SwapChainFramebuffers;
		uint32_t m_FrameIndex = 0;
		std::list<IApplicationLayer*> m_vApplicationLayers;

	};

	class IApplicationLayer
	{
	public:
		explicit IApplicationLayer(DeviceManager* deviceManager)
			: m_DeviceManager(deviceManager)
		{ }

		virtual ~IApplicationLayer() = default;

		virtual void Render(nvrhi::IFramebuffer* framebuffer) { }
		virtual void Animate(float elapsedSeconds) { }
		virtual void BackBufferResizing() { }
		virtual void BackBufferResized(const uint32_t width, uint32_t height, const uint32_t sampleCount) { }

		[[nodiscard]] DeviceManager* GetDeviceManager() const { return m_DeviceManager; }
		[[nodiscard]] nvrhi::IDevice* GetDevice() const { return m_DeviceManager->GetDevice(); }
		[[nodiscard]] uint32_t GetFrameIndex() const { return m_DeviceManager->GetFrameIndex(); }
	private:
		DeviceManager* m_DeviceManager;

	};
}