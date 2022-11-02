#include "Derydoca/Rendering/VK/DeviceManagerVK.h"
#include "Derydoca/Logging/Log.h"
#include "Derydoca/Rendering/VK/VKHelper.h"

#include <SDL2/SDL_vulkan.h>

//https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Instance

namespace DerydocaEngine::Rendering
{
	void DeviceManagerVK::Initialize(const DeviceManagerSettings& settings, SDL_Window* sdlWindow)
	{
		VkApplicationInfo appInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
		appInfo.pApplicationName = "Example";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Derydoca Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		{
			uint32_t availableExtensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
			std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());
			D_LOG_TRACE("Available VK Extensions: {}", availableExtensionCount);
			for (const auto& extension : availableExtensions)
			{
				D_LOG_TRACE("  {}", extension.extensionName);
			}
		}

		uint32_t extensionCount = 0;
		if (!SDL_Vulkan_GetInstanceExtensions(sdlWindow, &extensionCount, nullptr))
		{
			D_LOG_CRITICAL("Unable to get the number of required window extensions for the current platform!");
			exit(-1);
		}

		std::vector<const char*> extensions = {
			// Add default extensions here...
		};

		size_t additional_extension_count = extensions.size();
		extensions.resize(additional_extension_count + extensionCount);

		if (SDL_Vulkan_GetInstanceExtensions(sdlWindow, &extensionCount, extensions.data() + additional_extension_count) == SDL_FALSE)
		{
			D_LOG_CRITICAL("Unable to get Vulkan instance extensions!");
			exit(-1);
		}

		VkInstanceCreateInfo createInfo{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = extensionCount;
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.enabledLayerCount = 0;

		ThrowIfFailed(vkCreateInstance(&createInfo, nullptr, &instance));

		auto x = VK_SUCCESS;

		D_LOG_CRITICAL("NOT IMPLEMENTED!");
		exit(-1);
	}

	void DeviceManagerVK::Render()
	{
		D_LOG_CRITICAL("NOT IMPLEMENTED!");
		exit(-1);
	}

	void DeviceManagerVK::Cleanup()
	{
		vkDestroyInstance(instance, allocationCallbacks);
	}

}