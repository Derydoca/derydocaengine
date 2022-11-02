#pragma once

#include "Derydoca/DeviceManager.h"
#include <vulkan/vulkan.h>

namespace DerydocaEngine::Rendering
{
	class DeviceManagerVK : public DeviceManager
	{
	public:
		DeviceManagerVK() = default;
		DeviceManagerVK(const DeviceManagerVK&) = delete;

		void Initialize(const DeviceManagerSettings& settings, SDL_Window* sdlWindow) override;
		void Render() override;
		void Cleanup() override;

	private:
		VkInstance instance;

		VkAllocationCallbacks* allocationCallbacks = nullptr;

	};
}
