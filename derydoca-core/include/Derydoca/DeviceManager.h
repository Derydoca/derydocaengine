#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <nvrhi/nvrhi.h>
#ifdef USE_DX12
#include <dxgi.h>
#include <d3d12.h>
#endif

#include "Derydoca/Rendering/Common.h"
#include "Derydoca/Rendering/CommandBuffer.h"

namespace Derydoca::Rendering
{
	struct DeviceManagerSettings
	{
		uint32_t width = 1920;
		uint32_t height = 1080;
		uint32_t sampleCount = 1;
		uint32_t sampleQuality = 0;
        ImageFormat imageFormat = ImageFormat::RGBA8_UNORM;
		std::wstring adapterNameSubstring = L"";
		uint32_t bufferCount = 3;
		bool allowModeSwitch = true;
		bool enableDebugRuntime = true;
		bool enableComputeQueue = true;
		bool enableCopyQueue = true;
		uint32_t refreshRate = 0;
		bool startFullscreen = false;
		bool enableNvrhiValidationLayer = true;
		bool vsyncEnabled = false;

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

		static DeviceManager* Create(const RenderingAPI renderingAPI, const DeviceManagerSettings& settings, SDL_Window* sdlWindow);
		virtual ~DeviceManager() {};

        virtual void Render() = 0;
		virtual void CreateRenderPass(const RenderPassDesc& renderPassDesc, RenderPass* renderPass) = 0;
		virtual void CreateCommandBuffer(CommandBuffer* commandBuffer) const = 0;

		[[nodiscard]] virtual nvrhi::IDevice* GetDevice();
        void SignalWindowResizedEvent();

    protected:
		DeviceManager(const DeviceManagerSettings& deviceSettings, SDL_Window* sdlWindow);

		void UpdateWindowSize();
		void BackBufferResizing();
		void BackBufferResized();

		virtual nvrhi::GraphicsAPI GetGraphicsAPI() const = 0;
		virtual uint32_t GetBackBufferCount() = 0;
		virtual void ResizeSwapChain() = 0;
		virtual nvrhi::ITexture* GetBackBuffer(uint32_t index) = 0;

		DeviceManagerSettings m_DeviceSettings;
        SDL_Window* window;
        bool framebufferResized = false;
		bool m_windowVisible = false;
		bool m_RequestedVSync = false;


		nvrhi::DeviceHandle m_nvrhiDevice;
		std::vector<nvrhi::FramebufferHandle> m_SwapChainFramebuffers;

	};
}