#pragma once

#include "Derydoca/DeviceManager.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

namespace DerydocaEngine::Rendering
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;

		bool IsComplete()
		{
			return graphicsFamily.has_value();
		}
	};

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
		bool DeviceIsSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindDeviceQueues(VkPhysicalDevice device);

		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
		VkQueue graphicsQueue;

		VkAllocationCallbacks* allocationCallbacks = nullptr;

	};
}
