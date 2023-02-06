#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <nvrhi/nvrhi.h>

#include "Derydoca/Rendering/Common.h"
#include "Derydoca/Rendering/CommandBuffer.h"

namespace Derydoca::Rendering
{
	struct DeviceManagerSettings
	{
		uint32_t width = 1920;
		uint32_t height = 1080;
        ImageFormat imageFormat = ImageFormat::R8G8B8A8_UNORM;
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

        void SignalWindowResizedEvent();

    protected:
        SDL_Window* window;
        bool framebufferResized = false;

	private:
		virtual void Initialize(const DeviceManagerSettings& settings) = 0;
	};
}