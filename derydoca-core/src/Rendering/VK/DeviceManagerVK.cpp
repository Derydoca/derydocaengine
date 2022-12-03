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

		SubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = PipelineStageFlags::ColorAttachmentOutput;
		dependency.srcAccessMask = AccessFlags::None;
		dependency.dstStageMask = PipelineStageFlags::ColorAttachmentOutput;
		dependency.dstAccessMask = AccessFlags::ColorAttachmentWrite;

		RenderPassRenderTargetDesc renderTargetDesc{};
		renderTargetDesc.Format = Translate(swapChainImageFormat);
		renderTargetDesc.Samples = ImageSampleCount::_1;
		renderTargetDesc.BeginningAccess = RenderPassBeginningAccess::Clear;
		renderTargetDesc.EndingAccess = RenderPassEndingAccess::Preserve;
		renderTargetDesc.InitialLayout = ImageLayout::Undefined;
		renderTargetDesc.FinalLayout = ImageLayout::PresentSrc;

		AttachmentReference colorAttachmentRef{};
		colorAttachmentRef.Attachment = 0;
		colorAttachmentRef.Layout = ImageLayout::ColorAttachmentOptimal;

		SubpassDesc subpass{};
		subpass.BindPoint = PipelineBindPoint::Graphics;
		subpass.ColorAttachmentCount = 1;
		subpass.ColorAttachments = &colorAttachmentRef;

		RenderPassDesc renderPassDesc{};
		renderPassDesc.DependencyCount = 1;
		renderPassDesc.Dependencies = &dependency;
		renderPassDesc.AttachmentCount = 1;
		renderPassDesc.RenderTargets = &renderTargetDesc;
		renderPassDesc.SubpassCount = 1;
		renderPassDesc.Subpasses = &subpass;

		CreateRenderPass(renderPassDesc, static_cast<RenderPass*>(&mainRenderPass));

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
		renderPassInfo.renderPass = mainRenderPass;
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

		vkDestroyRenderPass(device, mainRenderPass, allocationCallbacks);

		vkDestroyDevice(device, allocationCallbacks);
#ifdef _DEBUG
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, allocationCallbacks);
#endif
		vkDestroySurfaceKHR(instance, surface, allocationCallbacks);
		vkDestroyInstance(instance, allocationCallbacks);
	}

	void DeviceManagerVK::CreateRenderPass(const RenderPassDesc& renderPassDesc, RenderPass* renderPass)
	{
		auto attachments = std::vector<VkAttachmentDescription>(renderPassDesc.AttachmentCount + (renderPassDesc.DepthStencil != nullptr ? 1 : 0));
		for (size_t attachmentIndex = 0; attachmentIndex < renderPassDesc.AttachmentCount; attachmentIndex++)
		{
			auto& attachment = renderPassDesc.RenderTargets[attachmentIndex];
			auto& vkAttachment = attachments[attachmentIndex];

			vkAttachment.finalLayout = Translate(attachment.FinalLayout);
			vkAttachment.flags = 0;
			vkAttachment.format = Translate(attachment.Format);
			vkAttachment.initialLayout = Translate(attachment.InitialLayout);
			vkAttachment.loadOp = Translate(attachment.BeginningAccess);
			vkAttachment.samples = Translate(attachment.Samples);
			vkAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			vkAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			vkAttachment.storeOp = Translate(attachment.EndingAccess);
		}

		auto subpasses = std::vector<VkSubpassDescription>(renderPassDesc.SubpassCount);
		auto colorAttachmentCollections = std::vector<std::vector<VkAttachmentReference>>(renderPassDesc.SubpassCount);
		auto resolveAttachmentCollections = std::vector<std::vector<VkAttachmentReference>>(renderPassDesc.SubpassCount);
		auto inputAttachmentCollections = std::vector<std::vector<VkAttachmentReference>>(renderPassDesc.SubpassCount);
		auto depthStencilAttachments = std::vector<VkAttachmentReference>(renderPassDesc.SubpassCount);
		for (size_t subpassIndex = 0; subpassIndex < renderPassDesc.SubpassCount; subpassIndex++)
		{
			auto& subpass = renderPassDesc.Subpasses[subpassIndex];
			auto& vkSubpass = subpasses[subpassIndex];

			auto& colorAttachments = colorAttachmentCollections[subpassIndex];
			auto& resolveAttachments = resolveAttachmentCollections[subpassIndex];
			colorAttachments.resize(subpass.ColorAttachmentCount);
			resolveAttachments.resize(subpass.ColorAttachmentCount);
			for (size_t attachmentIndex = 0; attachmentIndex < subpass.ColorAttachmentCount; attachmentIndex++)
			{
				auto& colorAttachment = subpass.ColorAttachments[attachmentIndex];
				auto& vkColorAttachment = colorAttachments[attachmentIndex];
				auto& vkResolveAttachment = resolveAttachments[attachmentIndex];

				vkColorAttachment.attachment = colorAttachment.Attachment;
				vkColorAttachment.layout = Translate(colorAttachment.Layout);

				// TODO: Add support for multisample resolve textures
				vkResolveAttachment.attachment = VK_ATTACHMENT_UNUSED;
				vkResolveAttachment.layout = VK_IMAGE_LAYOUT_UNDEFINED;
			}

			auto& inputAttachments = inputAttachmentCollections[subpassIndex];
			inputAttachments.resize(subpass.InputAttachmentCount);
			for (size_t attachmentIndex = 0; attachmentIndex < subpass.InputAttachmentCount; attachmentIndex++)
			{
				auto& inputAttachment = subpass.InputAttachments[attachmentIndex];
				auto& vkInputAttachment = inputAttachments[attachmentIndex];

				vkInputAttachment.attachment = inputAttachment.Attachment;
				vkInputAttachment.layout = Translate(inputAttachment.Layout);
			}

			auto& depthStencilAttachment = depthStencilAttachments[subpassIndex];
			if (subpass.DepthStencilAttachment != nullptr)
			{
				depthStencilAttachment.attachment = subpass.DepthStencilAttachment->Attachment;
				depthStencilAttachment.layout = Translate(subpass.DepthStencilAttachment->Layout);
			}
			else
			{
				depthStencilAttachment.attachment = VK_ATTACHMENT_UNUSED;
				depthStencilAttachment.layout = VK_IMAGE_LAYOUT_UNDEFINED;
			}

			vkSubpass.colorAttachmentCount = colorAttachments.size();
			vkSubpass.pColorAttachments = colorAttachments.data();
			vkSubpass.pResolveAttachments = resolveAttachments.data();
			vkSubpass.inputAttachmentCount = inputAttachments.size();
			vkSubpass.pInputAttachments = inputAttachments.data();
			vkSubpass.flags = 0;
			vkSubpass.pDepthStencilAttachment = &depthStencilAttachment;
			vkSubpass.pipelineBindPoint = Translate(subpass.BindPoint);
			vkSubpass.preserveAttachmentCount = subpass.PreserveAttachmentCount;
			vkSubpass.pPreserveAttachments = subpass.PreserveAttachments;
		}

		auto dependencies = std::vector<VkSubpassDependency>(renderPassDesc.DependencyCount);
		for (size_t dependencyIndex = 0; dependencyIndex < renderPassDesc.DependencyCount; dependencyIndex++)
		{
			auto& dependency = renderPassDesc.Dependencies[dependencyIndex];
			auto& vkDependency = dependencies[dependencyIndex];

			vkDependency.dependencyFlags = Translate(dependency.dependencyFlags);
			vkDependency.dstAccessMask = Translate(dependency.dstAccessMask);
			vkDependency.dstStageMask = Translate(dependency.dstStageMask);
			vkDependency.dstSubpass = dependency.dstSubpass;
			vkDependency.srcAccessMask = Translate(dependency.srcAccessMask);
			vkDependency.srcStageMask = Translate(dependency.srcStageMask);
			vkDependency.srcSubpass = dependency.srcSubpass;
		}

		VkRenderPassCreateInfo renderPassInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
		renderPassInfo.attachmentCount = attachments.size();
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = subpasses.size();
		renderPassInfo.pSubpasses = subpasses.data();
		renderPassInfo.dependencyCount = dependencies.size();
		renderPassInfo.pDependencies = dependencies.data();
		renderPassInfo.flags = 0;
		renderPassInfo.pNext = nullptr;

		ThrowIfFailed(vkCreateRenderPass(device, &renderPassInfo, nullptr, static_cast<VkRenderPass*>(renderPass)));
	}

	VkFormat DeviceManagerVK::Translate(const ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::UNKNOWN:
				return VK_FORMAT_UNDEFINED;
			//case ImageFormat::R32G32B32A32_TYPELESS:
			//	return VK_FORMAT_R32G32B32A32_TYPELESS;
			//case ImageFormat::R32G32B32A32_FLOAT:
			//	return VK_FORMAT_R32G32B32A32_FLOAT;
			case ImageFormat::R32G32B32A32_UINT:
				return VK_FORMAT_R32G32B32A32_UINT;
			case ImageFormat::R32G32B32A32_SINT:
				return VK_FORMAT_R32G32B32A32_SINT;
			//case ImageFormat::R32G32B32_TYPELESS:
			//	return VK_FORMAT_R32G32B32_TYPELESS;
			//case ImageFormat::R32G32B32_FLOAT:
			//	return VK_FORMAT_R32G32B32_FLOAT;
			case ImageFormat::R32G32B32_UINT:
				return VK_FORMAT_R32G32B32_UINT;
			case ImageFormat::R32G32B32_SINT:
				return VK_FORMAT_R32G32B32_SINT;
			//case ImageFormat::R16G16B16A16_TYPELESS:
			//	return VK_FORMAT_R16G16B16A16_TYPELESS;
			//case ImageFormat::R16G16B16A16_FLOAT:
			//	return VK_FORMAT_R16G16B16A16_FLOAT;
			case ImageFormat::R16G16B16A16_UNORM:
				return VK_FORMAT_R16G16B16A16_UNORM;
			case ImageFormat::R16G16B16A16_UINT:
				return VK_FORMAT_R16G16B16A16_UINT;
			case ImageFormat::R16G16B16A16_SNORM:
				return VK_FORMAT_R16G16B16A16_SNORM;
			case ImageFormat::R16G16B16A16_SINT:
				return VK_FORMAT_R16G16B16A16_SINT;
			//case ImageFormat::R32G32_TYPELESS:
			//	return VK_FORMAT_R32G32_TYPELESS;
			//case ImageFormat::R32G32_FLOAT:
			//	return VK_FORMAT_R32G32_FLOAT;
			case ImageFormat::R32G32_UINT:
				return VK_FORMAT_R32G32_UINT;
			case ImageFormat::R32G32_SINT:
				return VK_FORMAT_R32G32_SINT;
			case ImageFormat::R32G8X24_TYPELESS:
			//	return VK_FORMAT_R32G8X24_TYPELESS;
			//case ImageFormat::D32_FLOAT_S8X24_UINT:
			//	return VK_FORMAT_D32_FLOAT_S8X24_UINT;
			//case ImageFormat::R32_FLOAT_X8X24_TYPELESS:
			//	return VK_FORMAT_R32_FLOAT_X8X24_TYPELESS;
			//case ImageFormat::X32_TYPELESS_G8X24_UINT:
			//	return VK_FORMAT_X32_TYPELESS_G8X24_UINT;
			//case ImageFormat::R10G10B10A2_TYPELESS:
			//	return VK_FORMAT_R10G10B10A2_TYPELESS;
			//case ImageFormat::R10G10B10A2_UNORM:
			//	return VK_FORMAT_R10G10B10A2_UNORM;
			//case ImageFormat::R10G10B10A2_UINT:
			//	return VK_FORMAT_R10G10B10A2_UINT;
			//case ImageFormat::R11G11B10_FLOAT:
			//	return VK_FORMAT_R11G11B10_FLOAT;
			//case ImageFormat::R8G8B8A8_TYPELESS:
			//	return VK_FORMAT_R8G8B8A8_TYPELESS;
			case ImageFormat::R8G8B8A8_UNORM:
				return VK_FORMAT_R8G8B8A8_UNORM;
			//case ImageFormat::R8G8B8A8_UNORM_SRGB:
			//	return VK_FORMAT_R8G8B8A8_UNORM_SRGB;
			case ImageFormat::R8G8B8A8_UINT:
				return VK_FORMAT_R8G8B8A8_UINT;
			case ImageFormat::R8G8B8A8_SNORM:
				return VK_FORMAT_R8G8B8A8_SNORM;
			case ImageFormat::R8G8B8A8_SINT:
				return VK_FORMAT_R8G8B8A8_SINT;
			//case ImageFormat::R16G16_TYPELESS:
			//	return VK_FORMAT_R16G16_TYPELESS;
			//case ImageFormat::R16G16_FLOAT:
			//	return VK_FORMAT_R16G16_FLOAT;
			case ImageFormat::R16G16_UNORM:
				return VK_FORMAT_R16G16_UNORM;
			case ImageFormat::R16G16_UINT:
				return VK_FORMAT_R16G16_UINT;
			case ImageFormat::R16G16_SNORM:
				return VK_FORMAT_R16G16_SNORM;
			case ImageFormat::R16G16_SINT:
				return VK_FORMAT_R16G16_SINT;
			//case ImageFormat::R32_TYPELESS:
			//	return VK_FORMAT_R32_TYPELESS;
			//case ImageFormat::D32_FLOAT:
			//	return VK_FORMAT_D32_FLOAT;
			//case ImageFormat::R32_FLOAT:
			//	return VK_FORMAT_R32_FLOAT;
			case ImageFormat::R32_UINT:
				return VK_FORMAT_R32_UINT;
			case ImageFormat::R32_SINT:
				return VK_FORMAT_R32_SINT;
			//case ImageFormat::R24G8_TYPELESS:
			//	return VK_FORMAT_R24G8_TYPELESS;
			case ImageFormat::D24_UNORM_S8_UINT:
				return VK_FORMAT_D24_UNORM_S8_UINT;
			//case ImageFormat::R24_UNORM_X8_TYPELESS:
			//	return VK_FORMAT_R24_UNORM_X8_TYPELESS;
			//case ImageFormat::X24_TYPELESS_G8_UINT:
			//	return VK_FORMAT_X24_TYPELESS_G8_UINT;
			//case ImageFormat::R8G8_TYPELESS:
			//	return VK_FORMAT_R8G8_TYPELESS;
			case ImageFormat::R8G8_UNORM:
				return VK_FORMAT_R8G8_UNORM;
			case ImageFormat::R8G8_UINT:
				return VK_FORMAT_R8G8_UINT;
			case ImageFormat::R8G8_SNORM:
				return VK_FORMAT_R8G8_SNORM;
			case ImageFormat::R8G8_SINT:
				return VK_FORMAT_R8G8_SINT;
			//case ImageFormat::R16_TYPELESS:
			//	return VK_FORMAT_R16_TYPELESS;
			//case ImageFormat::R16_FLOAT:
			//	return VK_FORMAT_R16_FLOAT;
			case ImageFormat::D16_UNORM:
				return VK_FORMAT_D16_UNORM;
			case ImageFormat::R16_UNORM:
				return VK_FORMAT_R16_UNORM;
			case ImageFormat::R16_UINT:
				return VK_FORMAT_R16_UINT;
			case ImageFormat::R16_SNORM:
				return VK_FORMAT_R16_SNORM;
			case ImageFormat::R16_SINT:
				return VK_FORMAT_R16_SINT;
			//case ImageFormat::R8_TYPELESS:
			//	return VK_FORMAT_R8_TYPELESS;
			case ImageFormat::R8_UNORM:
				return VK_FORMAT_R8_UNORM;
			case ImageFormat::R8_UINT:
				return VK_FORMAT_R8_UINT;
			case ImageFormat::R8_SNORM:
				return VK_FORMAT_R8_SNORM;
			case ImageFormat::R8_SINT:
				return VK_FORMAT_R8_SINT;
			//case ImageFormat::A8_UNORM:
			//	return VK_FORMAT_A8_UNORM;
			//case ImageFormat::R1_UNORM:
			//	return VK_FORMAT_R1_UNORM;
			//case ImageFormat::R9G9B9E5_SHAREDEXP:
			//	return VK_FORMAT_R9G9B9E5_SHAREDEXP;
			//case ImageFormat::R8G8_B8G8_UNORM:
			//	return VK_FORMAT_R8G8_B8G8_UNORM;
			//case ImageFormat::G8R8_G8B8_UNORM:
			//	return VK_FORMAT_G8R8_G8B8_UNORM;
			//case ImageFormat::BC1_TYPELESS:
			//	return VK_FORMAT_BC1_TYPELESS;
			//case ImageFormat::BC1_UNORM:
			//	return VK_FORMAT_BC1_UNORM;
			//case ImageFormat::BC1_UNORM_SRGB:
			//	return VK_FORMAT_BC1_UNORM_SRGB;
			//case ImageFormat::BC2_TYPELESS:
			//	return VK_FORMAT_BC2_TYPELESS;
			//case ImageFormat::BC2_UNORM:
			//	return VK_FORMAT_BC2_UNORM;
			//case ImageFormat::BC2_UNORM_SRGB:
			//	return VK_FORMAT_BC2_UNORM_SRGB;
			//case ImageFormat::BC3_TYPELESS:
			//	return VK_FORMAT_BC3_TYPELESS;
			//case ImageFormat::BC3_UNORM:
			//	return VK_FORMAT_BC3_UNORM;
			//case ImageFormat::BC3_UNORM_SRGB:
			//	return VK_FORMAT_BC3_UNORM_SRGB;
			//case ImageFormat::BC4_TYPELESS:
			//	return VK_FORMAT_BC4_TYPELESS;
			//case ImageFormat::BC4_UNORM:
			//	return VK_FORMAT_BC4_UNORM;
			//case ImageFormat::BC4_SNORM:
			//	return VK_FORMAT_BC4_SNORM;
			//case ImageFormat::BC5_TYPELESS:
			//	return VK_FORMAT_BC5_TYPELESS;
			//case ImageFormat::BC5_UNORM:
			//	return VK_FORMAT_BC5_UNORM;
			//case ImageFormat::BC5_SNORM:
			//	return VK_FORMAT_BC5_SNORM;
			//case ImageFormat::B5G6R5_UNORM:
			//	return VK_FORMAT_B5G6R5_UNORM;
			//case ImageFormat::B5G5R5A1_UNORM:
			//	return VK_FORMAT_B5G5R5A1_UNORM;
			case ImageFormat::B8G8R8A8_UNORM:
				return VK_FORMAT_B8G8R8A8_UNORM;
			//case ImageFormat::B8G8R8X8_UNORM:
			//	return VK_FORMAT_B8G8R8X8_UNORM;
			//case ImageFormat::R10G10B10_XR_BIAS_A2_UNORM:
			//	return VK_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
			//case ImageFormat::B8G8R8A8_TYPELESS:
			//	return VK_FORMAT_B8G8R8A8_TYPELESS;
			case ImageFormat::B8G8R8A8_UNORM_SRGB:
				return VK_FORMAT_B8G8R8A8_SRGB;
			//case ImageFormat::B8G8R8X8_TYPELESS:
			//	return VK_FORMAT_B8G8R8X8_TYPELESS;
			//case ImageFormat::B8G8R8X8_UNORM_SRGB:
			//	return VK_FORMAT_B8G8R8X8_UNORM_SRGB;
			//case ImageFormat::BC6H_TYPELESS:
			//	return VK_FORMAT_BC6H_TYPELESS;
			//case ImageFormat::BC6H_UF16:
			//	return VK_FORMAT_BC6H_UF16;
			//case ImageFormat::BC6H_SF16:
			//	return VK_FORMAT_BC6H_SF16;
			//case ImageFormat::BC7_TYPELESS:
			//	return VK_FORMAT_BC7_TYPELESS;
			//case ImageFormat::BC7_UNORM:
			//	return VK_FORMAT_BC7_UNORM;
			//case ImageFormat::BC7_UNORM_SRGB:
			//	return VK_FORMAT_BC7_UNORM_SRGB;
			//case ImageFormat::AYUV:
			//	return VK_FORMAT_AYUV;
			//case ImageFormat::Y410:
			//	return VK_FORMAT_Y410;
			//case ImageFormat::Y416:
			//	return VK_FORMAT_Y416;
			//case ImageFormat::NV12:
			//	return VK_FORMAT_NV12;
			//case ImageFormat::P010:
			//	return VK_FORMAT_P010;
			//case ImageFormat::P016:
			//	return VK_FORMAT_P016;
			//case ImageFormat::YUV_420_OPAQUE:
			//	return VK_FORMAT_YUV_420_OPAQUE;
			//case ImageFormat::YUY2:
			//	return VK_FORMAT_YUY2;
			//case ImageFormat::Y210:
			//	return VK_FORMAT_Y210;
			//case ImageFormat::Y216:
			//	return VK_FORMAT_Y216;
			//case ImageFormat::NV11:
			//	return VK_FORMAT_NV11;
			//case ImageFormat::AI44:
			//	return VK_FORMAT_AI44;
			//case ImageFormat::IA44:
			//	return VK_FORMAT_IA44;
			//case ImageFormat::P8:
			//	return VK_FORMAT_P8;
			//case ImageFormat::A8P8:
			//	return VK_FORMAT_A8P8;
			//case ImageFormat::B4G4R4A4_UNORM:
			//	return VK_FORMAT_B4G4R4A4_UNORM;
		default:
			D_LOG_WARN("Unable to translate ImageFormat!");
			return VK_FORMAT_UNDEFINED;
		}
	}

	ImageFormat DeviceManagerVK::Translate(const VkFormat format)
	{
		switch (format)
		{
			case VK_FORMAT_UNDEFINED:
				return ImageFormat::UNKNOWN;
			//case VK_FORMAT_R4G4_UNORM_PACK8:
			//	return ImageFormat::R4G4_UNORM_PACK8;
			//case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
			//	return ImageFormat::R4G4B4A4_UNORM_PACK16;
			//case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
			//	return ImageFormat::B4G4R4A4_UNORM_PACK16;
			//case VK_FORMAT_R5G6B5_UNORM_PACK16:
			//	return ImageFormat::R5G6B5_UNORM_PACK16;
			//case VK_FORMAT_B5G6R5_UNORM_PACK16:
			//	return ImageFormat::B5G6R5_UNORM_PACK16;
			//case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
			//	return ImageFormat::R5G5B5A1_UNORM_PACK16;
			//case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
			//	return ImageFormat::B5G5R5A1_UNORM_PACK16;
			//case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
			//	return ImageFormat::A1R5G5B5_UNORM_PACK16;
			case VK_FORMAT_R8_UNORM:
				return ImageFormat::R8_UNORM;
			case VK_FORMAT_R8_SNORM:
				return ImageFormat::R8_SNORM;
			//case VK_FORMAT_R8_USCALED:
			//	return ImageFormat::R8_USCALED;
			//case VK_FORMAT_R8_SSCALED:
			//	return ImageFormat::R8_SSCALED;
			case VK_FORMAT_R8_UINT:
				return ImageFormat::R8_UINT;
			case VK_FORMAT_R8_SINT:
				return ImageFormat::R8_SINT;
			//case VK_FORMAT_R8_SRGB:
			//	return ImageFormat::R8_SRGB;
			case VK_FORMAT_R8G8_UNORM:
				return ImageFormat::R8G8_UNORM;
			case VK_FORMAT_R8G8_SNORM:
				return ImageFormat::R8G8_SNORM;
			//case VK_FORMAT_R8G8_USCALED:
			//	return ImageFormat::R8G8_USCALED;
			//case VK_FORMAT_R8G8_SSCALED:
			//	return ImageFormat::R8G8_SSCALED;
			case VK_FORMAT_R8G8_UINT:
				return ImageFormat::R8G8_UINT;
			case VK_FORMAT_R8G8_SINT:
				return ImageFormat::R8G8_SINT;
			//case VK_FORMAT_R8G8_SRGB:
			//	return ImageFormat::R8G8_SRGB;
			//case VK_FORMAT_R8G8B8_UNORM:
			//	return ImageFormat::R8G8B8_UNORM;
			//case VK_FORMAT_R8G8B8_SNORM:
			//	return ImageFormat::R8G8B8_SNORM;
			//case VK_FORMAT_R8G8B8_USCALED:
			//	return ImageFormat::R8G8B8_USCALED;
			//case VK_FORMAT_R8G8B8_SSCALED:
			//	return ImageFormat::R8G8B8_SSCALED;
			//case VK_FORMAT_R8G8B8_UINT:
			//	return ImageFormat::R8G8B8_UINT;
			//case VK_FORMAT_R8G8B8_SINT:
			//	return ImageFormat::R8G8B8_SINT;
			//case VK_FORMAT_R8G8B8_SRGB:
			//	return ImageFormat::R8G8B8_SRGB;
			//case VK_FORMAT_B8G8R8_UNORM:
			//	return ImageFormat::B8G8R8_UNORM;
			//case VK_FORMAT_B8G8R8_SNORM:
			//	return ImageFormat::B8G8R8_SNORM;
			//case VK_FORMAT_B8G8R8_USCALED:
			//	return ImageFormat::B8G8R8_USCALED;
			//case VK_FORMAT_B8G8R8_SSCALED:
			//	return ImageFormat::B8G8R8_SSCALED;
			//case VK_FORMAT_B8G8R8_UINT:
			//	return ImageFormat::B8G8R8_UINT;
			//case VK_FORMAT_B8G8R8_SINT:
			//	return ImageFormat::B8G8R8_SINT;
			//case VK_FORMAT_B8G8R8_SRGB:
			//	return ImageFormat::B8G8R8_SRGB;
			case VK_FORMAT_R8G8B8A8_UNORM:
				return ImageFormat::R8G8B8A8_UNORM;
			case VK_FORMAT_R8G8B8A8_SNORM:
				return ImageFormat::R8G8B8A8_SNORM;
			//case VK_FORMAT_R8G8B8A8_USCALED:
			//	return ImageFormat::R8G8B8A8_USCALED;
			//case VK_FORMAT_R8G8B8A8_SSCALED:
			//	return ImageFormat::R8G8B8A8_SSCALED;
			case VK_FORMAT_R8G8B8A8_UINT:
				return ImageFormat::R8G8B8A8_UINT;
			case VK_FORMAT_R8G8B8A8_SINT:
				return ImageFormat::R8G8B8A8_SINT;
			//case VK_FORMAT_R8G8B8A8_SRGB:
			//	return ImageFormat::R8G8B8A8_SRGB;
			case VK_FORMAT_B8G8R8A8_UNORM:
				return ImageFormat::B8G8R8A8_UNORM;
			//case VK_FORMAT_B8G8R8A8_SNORM:
			//	return ImageFormat::B8G8R8A8_SNORM;
			//case VK_FORMAT_B8G8R8A8_USCALED:
			//	return ImageFormat::B8G8R8A8_USCALED;
			//case VK_FORMAT_B8G8R8A8_SSCALED:
			//	return ImageFormat::B8G8R8A8_SSCALED;
			//case VK_FORMAT_B8G8R8A8_UINT:
			//	return ImageFormat::B8G8R8A8_UINT;
			//case VK_FORMAT_B8G8R8A8_SINT:
			//	return ImageFormat::B8G8R8A8_SINT;
			case VK_FORMAT_B8G8R8A8_SRGB:
				return ImageFormat::B8G8R8A8_UNORM_SRGB;
			//case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
			//	return ImageFormat::A8B8G8R8_UNORM_PACK32;
			//case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
			//	return ImageFormat::A8B8G8R8_SNORM_PACK32;
			//case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
			//	return ImageFormat::A8B8G8R8_USCALED_PACK32;
			//case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
			//	return ImageFormat::A8B8G8R8_SSCALED_PACK32;
			//case VK_FORMAT_A8B8G8R8_UINT_PACK32:
			//	return ImageFormat::A8B8G8R8_UINT_PACK32;
			//case VK_FORMAT_A8B8G8R8_SINT_PACK32:
			//	return ImageFormat::A8B8G8R8_SINT_PACK32;
			//case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
			//	return ImageFormat::A8B8G8R8_SRGB_PACK32;
			//case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
			//	return ImageFormat::A2R10G10B10_UNORM_PACK32;
			//case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
			//	return ImageFormat::A2R10G10B10_SNORM_PACK32;
			//case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
			//	return ImageFormat::A2R10G10B10_USCALED_PACK32;
			//case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
			//	return ImageFormat::A2R10G10B10_SSCALED_PACK32;
			//case VK_FORMAT_A2R10G10B10_UINT_PACK32:
			//	return ImageFormat::A2R10G10B10_UINT_PACK32;
			//case VK_FORMAT_A2R10G10B10_SINT_PACK32:
			//	return ImageFormat::A2R10G10B10_SINT_PACK32;
			//case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
			//	return ImageFormat::A2B10G10R10_UNORM_PACK32;
			//case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
			//	return ImageFormat::A2B10G10R10_SNORM_PACK32;
			//case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
			//	return ImageFormat::A2B10G10R10_USCALED_PACK32;
			//case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
			//	return ImageFormat::A2B10G10R10_SSCALED_PACK32;
			//case VK_FORMAT_A2B10G10R10_UINT_PACK32:
			//	return ImageFormat::A2B10G10R10_UINT_PACK32;
			//case VK_FORMAT_A2B10G10R10_SINT_PACK32:
			//	return ImageFormat::A2B10G10R10_SINT_PACK32;
			case VK_FORMAT_R16_UNORM:
				return ImageFormat::R16_UNORM;
			case VK_FORMAT_R16_SNORM:
				return ImageFormat::R16_SNORM;
			//case VK_FORMAT_R16_USCALED:
			//	return ImageFormat::R16_USCALED;
			//case VK_FORMAT_R16_SSCALED:
			//	return ImageFormat::R16_SSCALED;
			case VK_FORMAT_R16_UINT:
				return ImageFormat::R16_UINT;
			case VK_FORMAT_R16_SINT:
				return ImageFormat::R16_SINT;
			//case VK_FORMAT_R16_SFLOAT:
			//	return ImageFormat::R16_SFLOAT;
			case VK_FORMAT_R16G16_UNORM:
				return ImageFormat::R16G16_UNORM;
			case VK_FORMAT_R16G16_SNORM:
				return ImageFormat::R16G16_SNORM;
			//case VK_FORMAT_R16G16_USCALED:
			//	return ImageFormat::R16G16_USCALED;
			//case VK_FORMAT_R16G16_SSCALED:
			//	return ImageFormat::R16G16_SSCALED;
			case VK_FORMAT_R16G16_UINT:
				return ImageFormat::R16G16_UINT;
			case VK_FORMAT_R16G16_SINT:
				return ImageFormat::R16G16_SINT;
			//case VK_FORMAT_R16G16_SFLOAT:
			//	return ImageFormat::R16G16_SFLOAT;
			//case VK_FORMAT_R16G16B16_UNORM:
			//	return ImageFormat::R16G16B16_UNORM;
			//case VK_FORMAT_R16G16B16_SNORM:
			//	return ImageFormat::R16G16B16_SNORM;
			//case VK_FORMAT_R16G16B16_USCALED:
			//	return ImageFormat::R16G16B16_USCALED;
			//case VK_FORMAT_R16G16B16_SSCALED:
			//	return ImageFormat::R16G16B16_SSCALED;
			//case VK_FORMAT_R16G16B16_UINT:
			//	return ImageFormat::R16G16B16_UINT;
			//case VK_FORMAT_R16G16B16_SINT:
			//	return ImageFormat::R16G16B16_SINT;
			//case VK_FORMAT_R16G16B16_SFLOAT:
			//	return ImageFormat::R16G16B16_SFLOAT;
			case VK_FORMAT_R16G16B16A16_UNORM:
				return ImageFormat::R16G16B16A16_UNORM;
			case VK_FORMAT_R16G16B16A16_SNORM:
				return ImageFormat::R16G16B16A16_SNORM;
			//case VK_FORMAT_R16G16B16A16_USCALED:
			//	return ImageFormat::R16G16B16A16_USCALED;
			//case VK_FORMAT_R16G16B16A16_SSCALED:
			//	return ImageFormat::R16G16B16A16_SSCALED;
			case VK_FORMAT_R16G16B16A16_UINT:
				return ImageFormat::R16G16B16A16_UINT;
			case VK_FORMAT_R16G16B16A16_SINT:
				return ImageFormat::R16G16B16A16_SINT;
			//case VK_FORMAT_R16G16B16A16_SFLOAT:
			//	return ImageFormat::R16G16B16A16_SFLOAT;
			case VK_FORMAT_R32_UINT:
				return ImageFormat::R32_UINT;
			case VK_FORMAT_R32_SINT:
				return ImageFormat::R32_SINT;
			//case VK_FORMAT_R32_SFLOAT:
			//	return ImageFormat::R32_SFLOAT;
			case VK_FORMAT_R32G32_UINT:
				return ImageFormat::R32G32_UINT;
			case VK_FORMAT_R32G32_SINT:
				return ImageFormat::R32G32_SINT;
			//case VK_FORMAT_R32G32_SFLOAT:
			//	return ImageFormat::R32G32_SFLOAT;
			case VK_FORMAT_R32G32B32_UINT:
				return ImageFormat::R32G32B32_UINT;
			case VK_FORMAT_R32G32B32_SINT:
				return ImageFormat::R32G32B32_SINT;
			//case VK_FORMAT_R32G32B32_SFLOAT:
			//	return ImageFormat::R32G32B32_SFLOAT;
			case VK_FORMAT_R32G32B32A32_UINT:
				return ImageFormat::R32G32B32A32_UINT;
			case VK_FORMAT_R32G32B32A32_SINT:
				return ImageFormat::R32G32B32A32_SINT;
			//case VK_FORMAT_R32G32B32A32_SFLOAT:
			//	return ImageFormat::R32G32B32A32_SFLOAT;
			//case VK_FORMAT_R64_UINT:
			//	return ImageFormat::R64_UINT;
			//case VK_FORMAT_R64_SINT:
			//	return ImageFormat::R64_SINT;
			//case VK_FORMAT_R64_SFLOAT:
			//	return ImageFormat::R64_SFLOAT;
			//case VK_FORMAT_R64G64_UINT:
			//	return ImageFormat::R64G64_UINT;
			//case VK_FORMAT_R64G64_SINT:
			//	return ImageFormat::R64G64_SINT;
			//case VK_FORMAT_R64G64_SFLOAT:
			//	return ImageFormat::R64G64_SFLOAT;
			//case VK_FORMAT_R64G64B64_UINT:
			//	return ImageFormat::R64G64B64_UINT;
			//case VK_FORMAT_R64G64B64_SINT:
			//	return ImageFormat::R64G64B64_SINT;
			//case VK_FORMAT_R64G64B64_SFLOAT:
			//	return ImageFormat::R64G64B64_SFLOAT;
			//case VK_FORMAT_R64G64B64A64_UINT:
			//	return ImageFormat::R64G64B64A64_UINT;
			//case VK_FORMAT_R64G64B64A64_SINT:
			//	return ImageFormat::R64G64B64A64_SINT;
			//case VK_FORMAT_R64G64B64A64_SFLOAT:
			//	return ImageFormat::R64G64B64A64_SFLOAT;
			//case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
			//	return ImageFormat::B10G11R11_UFLOAT_PACK32;
			//case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
			//	return ImageFormat::E5B9G9R9_UFLOAT_PACK32;
			case VK_FORMAT_D16_UNORM:
				return ImageFormat::D16_UNORM;
			//case VK_FORMAT_X8_D24_UNORM_PACK32:
			//	return ImageFormat::X8_D24_UNORM_PACK32;
			//case VK_FORMAT_D32_SFLOAT:
			//	return ImageFormat::D32_SFLOAT;
			//case VK_FORMAT_S8_UINT:
			//	return ImageFormat::S8_UINT;
			//case VK_FORMAT_D16_UNORM_S8_UINT:
			//	return ImageFormat::D16_UNORM_S8_UINT;
			case VK_FORMAT_D24_UNORM_S8_UINT:
				return ImageFormat::D24_UNORM_S8_UINT;
			//case VK_FORMAT_D32_SFLOAT_S8_UINT:
			//	return ImageFormat::D32_SFLOAT_S8_UINT;
			//case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
			//	return ImageFormat::BC1_RGB_UNORM_BLOCK;
			//case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
			//	return ImageFormat::BC1_RGB_SRGB_BLOCK;
			//case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
			//	return ImageFormat::BC1_RGBA_UNORM_BLOCK;
			//case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
			//	return ImageFormat::BC1_RGBA_SRGB_BLOCK;
			//case VK_FORMAT_BC2_UNORM_BLOCK:
			//	return ImageFormat::BC2_UNORM_BLOCK;
			//case VK_FORMAT_BC2_SRGB_BLOCK:
			//	return ImageFormat::BC2_SRGB_BLOCK;
			//case VK_FORMAT_BC3_UNORM_BLOCK:
			//	return ImageFormat::BC3_UNORM_BLOCK;
			//case VK_FORMAT_BC3_SRGB_BLOCK:
			//	return ImageFormat::BC3_SRGB_BLOCK;
			//case VK_FORMAT_BC4_UNORM_BLOCK:
			//	return ImageFormat::BC4_UNORM_BLOCK;
			//case VK_FORMAT_BC4_SNORM_BLOCK:
			//	return ImageFormat::BC4_SNORM_BLOCK;
			//case VK_FORMAT_BC5_UNORM_BLOCK:
			//	return ImageFormat::BC5_UNORM_BLOCK;
			//case VK_FORMAT_BC5_SNORM_BLOCK:
			//	return ImageFormat::BC5_SNORM_BLOCK;
			//case VK_FORMAT_BC6H_UFLOAT_BLOCK:
			//	return ImageFormat::BC6H_UFLOAT_BLOCK;
			//case VK_FORMAT_BC6H_SFLOAT_BLOCK:
			//	return ImageFormat::BC6H_SFLOAT_BLOCK;
			//case VK_FORMAT_BC7_UNORM_BLOCK:
			//	return ImageFormat::BC7_UNORM_BLOCK;
			//case VK_FORMAT_BC7_SRGB_BLOCK:
			//	return ImageFormat::BC7_SRGB_BLOCK;
			//case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
			//	return ImageFormat::ETC2_R8G8B8_UNORM_BLOCK;
			//case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
			//	return ImageFormat::ETC2_R8G8B8_SRGB_BLOCK;
			//case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
			//	return ImageFormat::ETC2_R8G8B8A1_UNORM_BLOCK;
			//case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
			//	return ImageFormat::ETC2_R8G8B8A1_SRGB_BLOCK;
			//case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
			//	return ImageFormat::ETC2_R8G8B8A8_UNORM_BLOCK;
			//case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
			//	return ImageFormat::ETC2_R8G8B8A8_SRGB_BLOCK;
			//case VK_FORMAT_EAC_R11_UNORM_BLOCK:
			//	return ImageFormat::EAC_R11_UNORM_BLOCK;
			//case VK_FORMAT_EAC_R11_SNORM_BLOCK:
			//	return ImageFormat::EAC_R11_SNORM_BLOCK;
			//case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
			//	return ImageFormat::EAC_R11G11_UNORM_BLOCK;
			//case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
			//	return ImageFormat::EAC_R11G11_SNORM_BLOCK;
			//case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
			//	return ImageFormat::ASTC_4x4_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
			//	return ImageFormat::ASTC_4x4_SRGB_BLOCK;
			//case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
			//	return ImageFormat::ASTC_5x4_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
			//	return ImageFormat::ASTC_5x4_SRGB_BLOCK;
			//case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
			//	return ImageFormat::ASTC_5x5_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
			//	return ImageFormat::ASTC_5x5_SRGB_BLOCK;
			//case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
			//	return ImageFormat::ASTC_6x5_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
			//	return ImageFormat::ASTC_6x5_SRGB_BLOCK;
			//case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
			//	return ImageFormat::ASTC_6x6_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
			//	return ImageFormat::ASTC_6x6_SRGB_BLOCK;
			//case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
			//	return ImageFormat::ASTC_8x5_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
			//	return ImageFormat::ASTC_8x5_SRGB_BLOCK;
			//case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
			//	return ImageFormat::ASTC_8x6_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
			//	return ImageFormat::ASTC_8x6_SRGB_BLOCK;
			//case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
			//	return ImageFormat::ASTC_8x8_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
			//	return ImageFormat::ASTC_8x8_SRGB_BLOCK;
			//case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
			//	return ImageFormat::ASTC_10x5_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
			//	return ImageFormat::ASTC_10x5_SRGB_BLOCK;
			//case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
			//	return ImageFormat::ASTC_10x6_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
			//	return ImageFormat::ASTC_10x6_SRGB_BLOCK;
			//case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
			//	return ImageFormat::ASTC_10x8_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
			//	return ImageFormat::ASTC_10x8_SRGB_BLOCK;
			//case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
			//	return ImageFormat::ASTC_10x10_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
			//	return ImageFormat::ASTC_10x10_SRGB_BLOCK;
			//case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
			//	return ImageFormat::ASTC_12x10_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
			//	return ImageFormat::ASTC_12x10_SRGB_BLOCK;
			//case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
			//	return ImageFormat::ASTC_12x12_UNORM_BLOCK;
			//case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
			//	return ImageFormat::ASTC_12x12_SRGB_BLOCK;
			//case VK_FORMAT_G8B8G8R8_422_UNORM:
			//	return ImageFormat::G8B8G8R8_422_UNORM;
			//case VK_FORMAT_B8G8R8G8_422_UNORM:
			//	return ImageFormat::B8G8R8G8_422_UNORM;
			//case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM:
			//	return ImageFormat::G8_B8_R8_3PLANE_420_UNORM;
			//case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM:
			//	return ImageFormat::G8_B8R8_2PLANE_420_UNORM;
			//case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM:
			//	return ImageFormat::G8_B8_R8_3PLANE_422_UNORM;
			//case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM:
			//	return ImageFormat::G8_B8R8_2PLANE_422_UNORM;
			//case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM:
			//	return ImageFormat::G8_B8_R8_3PLANE_444_UNORM;
			//case VK_FORMAT_R10X6_UNORM_PACK16:
			//	return ImageFormat::R10X6_UNORM_PACK16;
			//case VK_FORMAT_R10X6G10X6_UNORM_2PACK16:
			//	return ImageFormat::R10X6G10X6_UNORM_2PACK16;
			//case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16:
			//	return ImageFormat::R10X6G10X6B10X6A10X6_UNORM_4PACK16;
			//case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
			//	return ImageFormat::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16;
			//case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
			//	return ImageFormat::B10X6G10X6R10X6G10X6_422_UNORM_4PACK16;
			//case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
			//	return ImageFormat::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16;
			//case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
			//	return ImageFormat::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16;
			//case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
			//	return ImageFormat::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16;
			//case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
			//	return ImageFormat::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16;
			//case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
			//	return ImageFormat::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16;
			//case VK_FORMAT_R12X4_UNORM_PACK16:
			//	return ImageFormat::R12X4_UNORM_PACK16;
			//case VK_FORMAT_R12X4G12X4_UNORM_2PACK16:
			//	return ImageFormat::R12X4G12X4_UNORM_2PACK16;
			//case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16:
			//	return ImageFormat::R12X4G12X4B12X4A12X4_UNORM_4PACK16;
			//case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
			//	return ImageFormat::G12X4B12X4G12X4R12X4_422_UNORM_4PACK16;
			//case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
			//	return ImageFormat::B12X4G12X4R12X4G12X4_422_UNORM_4PACK16;
			//case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
			//	return ImageFormat::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16;
			//case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
			//	return ImageFormat::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16;
			//case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
			//	return ImageFormat::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16;
			//case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
			//	return ImageFormat::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16;
			//case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
			//	return ImageFormat::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16;
			//case VK_FORMAT_G16B16G16R16_422_UNORM:
			//	return ImageFormat::G16B16G16R16_422_UNORM;
			//case VK_FORMAT_B16G16R16G16_422_UNORM:
			//	return ImageFormat::B16G16R16G16_422_UNORM;
			//case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM:
			//	return ImageFormat::G16_B16_R16_3PLANE_420_UNORM;
			//case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM:
			//	return ImageFormat::G16_B16R16_2PLANE_420_UNORM;
			//case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM:
			//	return ImageFormat::G16_B16_R16_3PLANE_422_UNORM;
			//case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM:
			//	return ImageFormat::G16_B16R16_2PLANE_422_UNORM;
			//case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM:
			//	return ImageFormat::G16_B16_R16_3PLANE_444_UNORM;
			//case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM:
			//	return ImageFormat::G8_B8R8_2PLANE_444_UNORM;
			//case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16:
			//	return ImageFormat::G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16;
			//case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16:
			//	return ImageFormat::G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16;
			//case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM:
			//	return ImageFormat::G16_B16R16_2PLANE_444_UNORM;
			//case VK_FORMAT_A4R4G4B4_UNORM_PACK16:
			//	return ImageFormat::A4R4G4B4_UNORM_PACK16;
			//case VK_FORMAT_A4B4G4R4_UNORM_PACK16:
			//	return ImageFormat::A4B4G4R4_UNORM_PACK16;
			//case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_4x4_SFLOAT_BLOCK;
			//case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_5x4_SFLOAT_BLOCK;
			//case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_5x5_SFLOAT_BLOCK;
			//case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_6x5_SFLOAT_BLOCK;
			//case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_6x6_SFLOAT_BLOCK;
			//case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_8x5_SFLOAT_BLOCK;
			//case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_8x6_SFLOAT_BLOCK;
			//case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_8x8_SFLOAT_BLOCK;
			//case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_10x5_SFLOAT_BLOCK;
			//case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_10x6_SFLOAT_BLOCK;
			//case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_10x8_SFLOAT_BLOCK;
			//case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_10x10_SFLOAT_BLOCK;
			//case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_12x10_SFLOAT_BLOCK;
			//case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK:
			//	return ImageFormat::ASTC_12x12_SFLOAT_BLOCK;
			//case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
			//	return ImageFormat::PVRTC1_2BPP_UNORM_BLOCK_IMG;
			//case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
			//	return ImageFormat::PVRTC1_4BPP_UNORM_BLOCK_IMG;
			//case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
			//	return ImageFormat::PVRTC2_2BPP_UNORM_BLOCK_IMG;
			//case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
			//	return ImageFormat::PVRTC2_4BPP_UNORM_BLOCK_IMG;
			//case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
			//	return ImageFormat::PVRTC1_2BPP_SRGB_BLOCK_IMG;
			//case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
			//	return ImageFormat::PVRTC1_4BPP_SRGB_BLOCK_IMG;
			//case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
			//	return ImageFormat::PVRTC2_2BPP_SRGB_BLOCK_IMG;
			//case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
			//	return ImageFormat::PVRTC2_4BPP_SRGB_BLOCK_IMG;
			//case VK_FORMAT_R16G16_S10_5_NV:
			//	return ImageFormat::R16G16_S10_5_NV;
		default:
			D_LOG_ERROR("Unable to translate VK image format!");
			return ImageFormat::UNKNOWN;
		}
	}

	VkAttachmentLoadOp DeviceManagerVK::Translate(const RenderPassBeginningAccess access)
	{
		switch (access)
		{
		case RenderPassBeginningAccess::Discard:
			return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		case RenderPassBeginningAccess::Preserve:
			return VK_ATTACHMENT_LOAD_OP_LOAD;
		case RenderPassBeginningAccess::Clear:
			return VK_ATTACHMENT_LOAD_OP_CLEAR;
		case RenderPassBeginningAccess::NoAccess:
			return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		default:
			D_LOG_ERROR("Unknown RenderPassBeginningAccess value!");
			return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		}
	}

	VkAttachmentStoreOp DeviceManagerVK::Translate(const RenderPassEndingAccess access)
	{
		switch (access)
		{
		case RenderPassEndingAccess::Discard:
			return VK_ATTACHMENT_STORE_OP_DONT_CARE;
		case RenderPassEndingAccess::Preserve:
			return VK_ATTACHMENT_STORE_OP_STORE;
		case RenderPassEndingAccess::Resolve:
			return VK_ATTACHMENT_STORE_OP_STORE;
		case RenderPassEndingAccess::NoAccess:
			return VK_ATTACHMENT_STORE_OP_NONE;
		default:
			D_LOG_ERROR("Unknown RenderPassEndingAccess value!");
			return VK_ATTACHMENT_STORE_OP_DONT_CARE;
		}
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
			framebufferInfo.renderPass = mainRenderPass;
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