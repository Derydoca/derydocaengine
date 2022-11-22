#include "Derydoca/Rendering/VK/DeviceManagerVK.h"
#include "Derydoca/Logging/Log.h"
#include "Derydoca/Rendering/VK/VKHelper.h"

#include <SDL2/SDL_vulkan.h>
#include <set>
#include <limits>
#include <algorithm>

//https://vulkan-tutorial.com/en/Drawing_a_triangle/Drawing/Frames_in_flight

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace Derydoca::Rendering
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

	VkSurfaceKHR surface;

	void DeviceManagerVK::Initialize(const DeviceManagerSettings& settings)
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
		if (!SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr))
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

		if (SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data() + additional_extension_count) == SDL_FALSE)
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
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
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

		// Create surface
		if (SDL_Vulkan_CreateSurface(window, instance, &surface) == SDL_FALSE)
		{
			D_LOG_CRITICAL("Unable to create Vulkan window surface!");
			exit(-1);
		}

		// Pick the device
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0)
		{
			D_LOG_CRITICAL("No physical devices support Vulkan on this machine!");
			exit(-1);
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		for (const auto& device : devices)
		{
			if (DeviceIsSuitable(device))
			{
				physicalDevice = device;
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE)
		{
			D_LOG_CRITICAL("Failed to find a suitable GPU!");
			exit(-1);
		}

		// Create the logical device
		QueueFamilyIndices indices = FindDeviceQueues(physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;

		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo deviceCreateInfo{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

		ThrowIfFailed(vkCreateDevice(physicalDevice, &deviceCreateInfo, allocationCallbacks, &device));

		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

		CreateSwapChain();

		CreateImageViews();

		// Create render pass
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // Change for multisampling
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		ThrowIfFailed(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass));

		CreateFramebuffers();

		// Create command pool
		VkCommandPoolCreateInfo poolInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = indices.graphicsFamily.value();

		ThrowIfFailed(vkCreateCommandPool(device, &poolInfo, allocationCallbacks, &renderingCommandPool));

		// Create sync objects
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

		VkFenceCreateInfo fenceInfo{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
		// Start the fence with it signaled so the first call to Render isn't waiting indefinitely since vkWaitForFences(...) will never fire on the first call.
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (vkCreateSemaphore(device, &semaphoreInfo, allocationCallbacks, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(device, &semaphoreInfo, allocationCallbacks, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(device, &fenceInfo, allocationCallbacks, &inFlightFences[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create semaphores!");
			}
		}
	}

	void DeviceManagerVK::Render()
	{
		vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex = 0;
		VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
		{
			framebufferResized = false;
			RecreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to aquire the swap chain image!");
		}

		vkResetFences(device, 1, &inFlightFences[currentFrame]);

		VkCommandBufferAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		allocInfo.commandPool = renderingCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		// DX12 creates command buffers in the recording state so we should do the same here
		VkCommandBuffer commandBuffer;
		{
			if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS)
			{
				throw("Failed to allocate command buffers!");
			}

			VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
			beginInfo.flags = 0;
			beginInfo.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to begin recording command buffer!");
			}
		}

		{
			VkViewport viewport{};
			viewport.height = 1080;
			viewport.width = 1920;
			viewport.minDepth = 0.0;
			viewport.maxDepth = 1.0;

			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		}

		{
			VkRect2D rect{};
			rect.offset.x = 0;
			rect.offset.y = 0;
			rect.extent.width = 1080;
			rect.extent.height = 1920;

			vkCmdSetScissor(commandBuffer, 0, 1, &rect);
		}

		VkRenderPassBeginInfo renderPassInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent;

		static float t = 0.0f;
		t += 0.005f;
		float r = (sin(t) + 1.0) * 0.5f;
		VkClearValue clearColor = { {{r, 0.2f, 0.4f, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdEndRenderPass(commandBuffer);
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to record command buffer!");
		}

		VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(presentQueue, &presentInfo);

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void DeviceManagerVK::Cleanup()
	{
		vkDeviceWaitIdle(device);

		CleanupSwapChain();

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(device, imageAvailableSemaphores[i], allocationCallbacks);
			vkDestroySemaphore(device, renderFinishedSemaphores[i], allocationCallbacks);
			vkDestroyFence(device, inFlightFences[i], allocationCallbacks);
		}

		vkDestroyCommandPool(device, renderingCommandPool, allocationCallbacks);

		vkDestroyRenderPass(device, renderPass, allocationCallbacks);

		vkDestroyDevice(device, allocationCallbacks);
#ifdef _DEBUG
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, allocationCallbacks);
#endif
		vkDestroySurfaceKHR(instance, surface, allocationCallbacks);
		vkDestroyInstance(instance, allocationCallbacks);
	}

	void DeviceManagerVK::CleanupSwapChain()
	{
		for (auto framebuffer : swapChainFramebuffers)
		{
			vkDestroyFramebuffer(device, framebuffer, allocationCallbacks);
		}

		for (auto imageView : swapChainImageViews)
		{
			vkDestroyImageView(device, imageView, allocationCallbacks);
		}

		vkDestroySwapchainKHR(device, swapChain, allocationCallbacks);
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
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;
	}

	bool DeviceManagerVK::DeviceIsSuitable(VkPhysicalDevice device)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		QueueFamilyIndices indices = FindDeviceQueues(device);

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
			&& deviceFeatures.geometryShader
			&& indices.IsComplete()
			&& extensionsSupported
			&& swapChainAdequate;
	}

	bool DeviceManagerVK::checkDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	QueueFamilyIndices DeviceManagerVK::FindDeviceQueues(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}

			if (indices.IsComplete())
			{
				break;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			if (presentSupport)
			{
				indices.presentFamily = i;
			}

			i++;
		}

		return indices;
	}

	SwapChainSupportDetails DeviceManagerVK::QuerySwapChainSupport(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
		
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR DeviceManagerVK::ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats, VkFormat preferredFormat, VkColorSpaceKHR preferredColorSpace)
	{
		for each (const auto& availableFormat in availableFormats)
		{
			if (availableFormat.format == preferredFormat && availableFormat.colorSpace == preferredColorSpace)
			{
				return availableFormat;
			}
		}

		D_LOG_WARN("Unable to find the preferred swap chain surface format. Defaulting to the first one presented.");

		return availableFormats[0];
	}

	VkPresentModeKHR DeviceManagerVK::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for each (const auto& availablePresentMode in availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D DeviceManagerVK::ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR& capabilities, SDL_Window* window)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			int width, height;
			SDL_Vulkan_GetDrawableSize(window, &width, &height);

			VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	void DeviceManagerVK::CreateSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapChainSurfaceFormat(swapChainSupport.formats, VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
		VkPresentModeKHR presentMode = ChooseSwapChainPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = ChooseSwapChainExtent(swapChainSupport.capabilities, window);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR swapchainCreateInfo{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
		swapchainCreateInfo.surface = surface;
		swapchainCreateInfo.minImageCount = imageCount;
		swapchainCreateInfo.imageFormat = surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = extent;
		swapchainCreateInfo.imageArrayLayers = 1; // Use 2 for stereoscopic image generation (VR?)
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // Use VK_IMAGE_USAGE_TRANSFER_DST_BIT when we want to render to an intermediate buffer first
		//swapchainCreateInfo.oldSwapchain = swapChain;

		QueueFamilyIndices indices = FindDeviceQueues(physicalDevice);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
		if (indices.graphicsFamily != indices.presentFamily)
		{
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainCreateInfo.queueFamilyIndexCount = 2;
			swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainCreateInfo.queueFamilyIndexCount = 0;
			swapchainCreateInfo.pQueueFamilyIndices = nullptr;
		}
		swapchainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = presentMode;
		swapchainCreateInfo.clipped = VK_TRUE; // Set to VK_FALSE if we need to read back pixels on a window that is obscured by another
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(device, &swapchainCreateInfo, allocationCallbacks, &swapChain) != VK_SUCCESS)
		{
			D_LOG_CRITICAL("Failed to create the swapchain!");
			exit(-1);
		}

		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;
	}

	void DeviceManagerVK::CreateImageViews()
	{
		swapChainImageViews.resize(swapChainImages.size());

		for (size_t i = 0; i < swapChainImages.size(); i++)
		{
			VkImageViewCreateInfo imageViewCreateInfo{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
			imageViewCreateInfo.image = swapChainImages[i];
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = swapChainImageFormat;
			imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1; // Use multiple layers for stereoscopic 3D (VR?)

			if (vkCreateImageView(device, &imageViewCreateInfo, allocationCallbacks, &swapChainImageViews[i]) != VK_SUCCESS)
			{
				D_LOG_CRITICAL("Failed to create image views!");
				exit(-1);
			}
		}
	}

	void DeviceManagerVK::CreateFramebuffers()
	{
		swapChainFramebuffers.resize(swapChainImageViews.size());
		for (size_t i = 0; i < swapChainImageViews.size(); i++)
		{
			VkImageView attachments[] =
			{
				swapChainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
			framebufferInfo.renderPass = renderPass; // We don't have a render pass to bind
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(device, &framebufferInfo, allocationCallbacks, &swapChainFramebuffers[i]) != VK_SUCCESS)
			{
				D_LOG_CRITICAL("Failed to create framebuffer!");
				exit(-1);
			}
		}
	}

	void DeviceManagerVK::RecreateSwapChain()
	{
		// If the window is minimized, wait for it to be restored because width and height will be zero values which is invalid.
		int width = 0, height = 0;
		SDL_Vulkan_GetDrawableSize(window, &width, &height);
		while (width <= 0 || height <= 0)
		{
			SDL_Vulkan_GetDrawableSize(window, &width, &height);
			SDL_Event evt;
			SDL_WaitEvent(&evt);
		}

		vkDeviceWaitIdle(device);

		CleanupSwapChain();

		CreateSwapChain();
		CreateImageViews();
		CreateFramebuffers();
	}
}