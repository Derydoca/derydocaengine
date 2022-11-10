#include "Derydoca/Rendering/VK/DeviceManagerVK.h"
#include "Derydoca/Logging/Log.h"
#include "Derydoca/Rendering/VK/VKHelper.h"

#include <SDL2/SDL_vulkan.h>

//https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Physical_devices_and_queue_families

namespace DerydocaEngine::Rendering
{
	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT type,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		char* typeStr = "Unknown Type";
		switch (type)
		{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			typeStr = "General";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			typeStr = "Validation";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			typeStr = "Performance";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
			typeStr = "Device Address Binding";
			break;
		}

		if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			D_LOG_ERROR("VK Validation [{}]: {}", typeStr, pCallbackData->pMessage);
		}
		else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			D_LOG_WARN("VK Validation [{}]: {}", typeStr, pCallbackData->pMessage);
		}
		else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		{
			D_LOG_INFO("VK Validation [{}]: {}", typeStr, pCallbackData->pMessage);
		}
		else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
		{
			D_LOG_TRACE("VK Validation [{}]: {}", typeStr, pCallbackData->pMessage);
		}
		else
		{
			D_LOG_ERROR("VK Validation [SEVERITY UNKNOWN] [{}]: {}", typeStr, pCallbackData->pMessage);
		}

		return VK_FALSE;
	}

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
#ifdef _DEBUG
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
		};

		size_t additional_extension_count = extensions.size();
		extensions.resize(additional_extension_count + extensionCount);

		if (SDL_Vulkan_GetInstanceExtensions(sdlWindow, &extensionCount, extensions.data() + additional_extension_count) == SDL_FALSE)
		{
			D_LOG_CRITICAL("Unable to get Vulkan instance extensions!");
			exit(-1);
		}

		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

#ifdef _DEBUG
		if (!CheckValidationLayerSupport(validationLayers))
		{
			D_LOG_CRITICAL("Vulkan validation layers are not found but are required to run in debug mode!");
			exit(-1);
		}
#endif

		VkInstanceCreateInfo createInfo { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = extensionCount;
		createInfo.ppEnabledExtensionNames = extensions.data();
#ifdef _DEBUG
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
#endif

		VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo;
		PopulateDebugMessengerCreateInfo(messengerCreateInfo);

		ThrowIfFailed(vkCreateInstance(&createInfo, nullptr, &instance));

		ThrowIfFailed(CreateDebugUtilsMessengerEXT(instance, &messengerCreateInfo, allocationCallbacks, &debugMessenger));

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
#ifdef _DEBUG
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, allocationCallbacks);
#endif
		vkDestroyInstance(instance, allocationCallbacks);
	}

	bool DeviceManagerVK::CheckValidationLayerSupport(const std::vector<const char*>& validationLayers)
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}

		return true;
	}

	VkResult DeviceManagerVK::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DeviceManagerVK::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}

	void DeviceManagerVK::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;
	}
}