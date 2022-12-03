#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "Derydoca/Rendering/Common.h"
#include "Derydoca/Rendering/RenderPass.h"

namespace Derydoca::Rendering
{
	struct DeviceManagerSettings
	{
		uint32_t width = 1920;
		uint32_t height = 1080;
        ImageFormat imageFormat = ImageFormat::R8G8B8A8_UNORM;
	};

	class DeviceManager
	{
	public:
		static DeviceManager* Create(const RenderingAPI renderingAPI, const DeviceManagerSettings& settings, SDL_Window* sdlWindow);

		void Initialize();
        virtual void Render() = 0;
        virtual void Cleanup() = 0;
		virtual void CreateRenderPass(const RenderPassDesc& renderPassDesc, RenderPass* renderPass) = 0;

        void SignalWindowResizedEvent();

    protected:
        SDL_Window* window;
        bool framebufferResized = false;

	private:
		virtual void Initialize(const DeviceManagerSettings& settings) = 0;
	};
}