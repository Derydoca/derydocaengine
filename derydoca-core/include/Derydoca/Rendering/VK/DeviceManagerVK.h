#pragma once

#include "Derydoca/DeviceManager.h"
#include <vulkan/vulkan.h>
#include <vector>

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
		bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;

		VkAllocationCallbacks* allocationCallbacks = nullptr;

	};
}
