#include "Derydoca/Rendering/VK/DeviceManagerVK.h"
#include "Derydoca/Logging/Log.h"
#include "Derydoca/Rendering/VK/VKHelper.h"
#include "Derydoca/Rendering/VK/CommandBufferVK.h"

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

	DeviceManagerVK::DeviceManagerVK(const DeviceManagerSettings& deviceSettings)
		: DeviceManager(deviceSettings)
	{
		//Initialize(deviceSettings);
	}

	DeviceManagerVK::~DeviceManagerVK()
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

	nvrhi::GraphicsAPI DeviceManagerVK::GetGraphicsAPI() const
	{
		return nvrhi::GraphicsAPI::VULKAN;
	}

	uint32_t DeviceManagerVK::GetBackBufferCount()
	{
		return 0;
	}

	void DeviceManagerVK::ResizeSwapChain()
	{
	}

	nvrhi::ITexture* DeviceManagerVK::GetBackBuffer(uint32_t index)
	{
		return nullptr;
	}
	
	bool DeviceManagerVK::CreateDeviceAndSwapChain()
	{
		return false;
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

#if 0
		CommandBuffer* cmd = nullptr;
		CreateCommandBuffer(cmd);

		Viewport viewport;
		viewport.height = 1080;
		viewport.width = 1920;
		viewport.minDepth = 0.0;
		viewport.maxDepth = 1.0;
		viewport.x = 0.0;
		viewport.y = 0.0;
		cmd->SetViewport(viewport);

		RectI scissorRect;
		scissorRect.extent = { 1920, 1080 };
		scissorRect.offset = { 0, 0 };
		cmd->SetScissorRect(scissorRect);

		static float t = 0.0f;
		t += 0.05f;
		float b = (sin(t) + 1.0) * 0.5f;
		const float clearColor[] = { 0.2, 0.1, b, 1.0f };

		int numTargets = 1;
		auto clearValues = std::vector<ClearValue>(numTargets);
		clearValues[0].Color.float32[0] = clearColor[0];
		clearValues[0].Color.float32[1] = clearColor[1];
		clearValues[0].Color.float32[2] = clearColor[2];
		clearValues[0].Color.float32[3] = clearColor[3];

		auto targets = std::vector<RenderTarget*>(numTargets);
		targets[0] = &swapChainFramebuffers[imageIndex];

		RenderPassBeginInfo beginInfo{};
		beginInfo.Area.offset = { 0,0 };
		beginInfo.Area.extent = { swapChainExtent.width, swapChainExtent.height };
		beginInfo.ClearValueCount = clearValues.size();
		beginInfo.ClearValues = clearValues.data();
		beginInfo.Targets = targets.data();

		cmd->BeginRenderPass(mainRenderPass, beginInfo);

		cmd->EndRenderPass();

		// TODO: Wrap up the command submit info data into a rendering API agnostic structure
		SubmitCommandBuffer(cmd, submitInfo);
#else
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

		{
			static float t = 0.0f;
			t += 0.05f;
			float b = (sin(t) + 1.0) * 0.5f;
			const float clearColor[] = { 0.2, 0.1, b, 1.0f };

			int numTargets = 1;
			auto clearValues = std::vector<ClearValue>(numTargets);
			clearValues[0].Color.float32[0] = clearColor[0];
			clearValues[0].Color.float32[1] = clearColor[1];
			clearValues[0].Color.float32[2] = clearColor[2];
			clearValues[0].Color.float32[3] = clearColor[3];

			auto targets = std::vector<RenderTarget*>(numTargets);
			targets[0] = &swapChainFramebuffers[imageIndex];

			RenderPassBeginInfo beginInfo{};
			beginInfo.Area.offset = { 0,0 };
			beginInfo.Area.extent = { swapChainExtent.width, swapChainExtent.height };
			beginInfo.ClearValueCount = clearValues.size();
			beginInfo.ClearValues = clearValues.data();
			beginInfo.Targets = targets.data();

			// Command buffer begin render pass: mainRenderPass, beginInfo

			auto framebuffer = static_cast<VkFramebuffer*>(beginInfo.Targets[0]);

			bool hasDepthStencil = beginInfo.DepthStencil != nullptr;
			auto clearValuesVK = std::vector<VkClearValue>(beginInfo.ClearValueCount + (hasDepthStencil ? 1 : 0));
			for (size_t clearValueIndex = 0; clearValueIndex < beginInfo.ClearValueCount; clearValueIndex++)
			{
				auto& clearValueVK = clearValuesVK[clearValueIndex];
				auto& clearValue = beginInfo.ClearValues[clearValueIndex];

				for (size_t colorIndex = 0; colorIndex < 4; colorIndex++)
				{
					clearValueVK.color.float32[colorIndex] = clearValue.Color.float32[colorIndex];
					clearValueVK.color.int32[colorIndex] = clearValue.Color.int32[colorIndex];
					clearValueVK.color.uint32[colorIndex] = clearValue.Color.uint32[colorIndex];
				}
			}

			if (hasDepthStencil)
			{
				auto& clearValueVK = clearValuesVK[clearValues.size() - 1];

				clearValueVK.depthStencil.depth = beginInfo.DepthStencil->DepthStencil.Depth;
				clearValueVK.depthStencil.stencil = beginInfo.DepthStencil->DepthStencil.Stencil;
			}

			VkRenderPassBeginInfo renderPassInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
			renderPassInfo.renderPass = mainRenderPass;
			renderPassInfo.framebuffer = *framebuffer;
			renderPassInfo.renderArea.offset = { beginInfo.Area.offset.x, beginInfo.Area.offset.y };
			renderPassInfo.renderArea.extent = { beginInfo.Area.extent.x, beginInfo.Area.extent.y };
			renderPassInfo.pNext = nullptr;
			renderPassInfo.clearValueCount = clearValuesVK.size();
			renderPassInfo.pClearValues = clearValuesVK.data();

			vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		}

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
#endif

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void DeviceManagerVK::CreateRenderPass(const RenderPassDesc& renderPassDesc, RenderPass* renderPass)
	{
		//auto attachments = std::vector<VkAttachmentDescription>(renderPassDesc.AttachmentCount + (renderPassDesc.DepthStencil != nullptr ? 1 : 0));
		//for (size_t attachmentIndex = 0; attachmentIndex < renderPassDesc.AttachmentCount; attachmentIndex++)
		//{
		//	auto& attachment = renderPassDesc.RenderTargets[attachmentIndex];
		//	auto& vkAttachment = attachments[attachmentIndex];

		//	vkAttachment.finalLayout = Translate(attachment.FinalLayout);
		//	vkAttachment.flags = 0;
		//	vkAttachment.format = Translate(attachment.Format);
		//	vkAttachment.initialLayout = Translate(attachment.InitialLayout);
		//	vkAttachment.loadOp = Translate(attachment.BeginningAccess);
		//	vkAttachment.samples = Translate(attachment.Samples);
		//	vkAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		//	vkAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		//	vkAttachment.storeOp = Translate(attachment.EndingAccess);
		//}

		//auto subpasses = std::vector<VkSubpassDescription>(renderPassDesc.SubpassCount);
		//auto colorAttachmentCollections = std::vector<std::vector<VkAttachmentReference>>(renderPassDesc.SubpassCount);
		//auto resolveAttachmentCollections = std::vector<std::vector<VkAttachmentReference>>(renderPassDesc.SubpassCount);
		//auto inputAttachmentCollections = std::vector<std::vector<VkAttachmentReference>>(renderPassDesc.SubpassCount);
		//auto depthStencilAttachments = std::vector<VkAttachmentReference>(renderPassDesc.SubpassCount);
		//for (size_t subpassIndex = 0; subpassIndex < renderPassDesc.SubpassCount; subpassIndex++)
		//{
		//	auto& subpass = renderPassDesc.Subpasses[subpassIndex];
		//	auto& vkSubpass = subpasses[subpassIndex];

		//	auto& colorAttachments = colorAttachmentCollections[subpassIndex];
		//	auto& resolveAttachments = resolveAttachmentCollections[subpassIndex];
		//	colorAttachments.resize(subpass.ColorAttachmentCount);
		//	resolveAttachments.resize(subpass.ColorAttachmentCount);
		//	for (size_t attachmentIndex = 0; attachmentIndex < subpass.ColorAttachmentCount; attachmentIndex++)
		//	{
		//		auto& colorAttachment = subpass.ColorAttachments[attachmentIndex];
		//		auto& vkColorAttachment = colorAttachments[attachmentIndex];
		//		auto& vkResolveAttachment = resolveAttachments[attachmentIndex];

		//		vkColorAttachment.attachment = colorAttachment.Attachment;
		//		vkColorAttachment.layout = Translate(colorAttachment.Layout);

		//		// TODO: Add support for multisample resolve textures
		//		vkResolveAttachment.attachment = VK_ATTACHMENT_UNUSED;
		//		vkResolveAttachment.layout = VK_IMAGE_LAYOUT_UNDEFINED;
		//	}

		//	auto& inputAttachments = inputAttachmentCollections[subpassIndex];
		//	inputAttachments.resize(subpass.InputAttachmentCount);
		//	for (size_t attachmentIndex = 0; attachmentIndex < subpass.InputAttachmentCount; attachmentIndex++)
		//	{
		//		auto& inputAttachment = subpass.InputAttachments[attachmentIndex];
		//		auto& vkInputAttachment = inputAttachments[attachmentIndex];

		//		vkInputAttachment.attachment = inputAttachment.Attachment;
		//		vkInputAttachment.layout = Translate(inputAttachment.Layout);
		//	}

		//	auto& depthStencilAttachment = depthStencilAttachments[subpassIndex];
		//	if (subpass.DepthStencilAttachment != nullptr)
		//	{
		//		depthStencilAttachment.attachment = subpass.DepthStencilAttachment->Attachment;
		//		depthStencilAttachment.layout = Translate(subpass.DepthStencilAttachment->Layout);
		//	}
		//	else
		//	{
		//		depthStencilAttachment.attachment = VK_ATTACHMENT_UNUSED;
		//		depthStencilAttachment.layout = VK_IMAGE_LAYOUT_UNDEFINED;
		//	}

		//	vkSubpass.colorAttachmentCount = colorAttachments.size();
		//	vkSubpass.pColorAttachments = colorAttachments.data();
		//	vkSubpass.pResolveAttachments = resolveAttachments.data();
		//	vkSubpass.inputAttachmentCount = inputAttachments.size();
		//	vkSubpass.pInputAttachments = inputAttachments.data();
		//	vkSubpass.flags = 0;
		//	vkSubpass.pDepthStencilAttachment = &depthStencilAttachment;
		//	vkSubpass.pipelineBindPoint = Translate(subpass.BindPoint);
		//	vkSubpass.preserveAttachmentCount = subpass.PreserveAttachmentCount;
		//	vkSubpass.pPreserveAttachments = subpass.PreserveAttachments;
		//}

		//auto dependencies = std::vector<VkSubpassDependency>(renderPassDesc.DependencyCount);
		//for (size_t dependencyIndex = 0; dependencyIndex < renderPassDesc.DependencyCount; dependencyIndex++)
		//{
		//	auto& dependency = renderPassDesc.Dependencies[dependencyIndex];
		//	auto& vkDependency = dependencies[dependencyIndex];

		//	vkDependency.dependencyFlags = Translate(dependency.dependencyFlags);
		//	vkDependency.dstAccessMask = Translate(dependency.dstAccessMask);
		//	vkDependency.dstStageMask = Translate(dependency.dstStageMask);
		//	vkDependency.dstSubpass = dependency.dstSubpass;
		//	vkDependency.srcAccessMask = Translate(dependency.srcAccessMask);
		//	vkDependency.srcStageMask = Translate(dependency.srcStageMask);
		//	vkDependency.srcSubpass = dependency.srcSubpass;
		//}

		//VkRenderPassCreateInfo renderPassInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
		//renderPassInfo.attachmentCount = attachments.size();
		//renderPassInfo.pAttachments = attachments.data();
		//renderPassInfo.subpassCount = subpasses.size();
		//renderPassInfo.pSubpasses = subpasses.data();
		//renderPassInfo.dependencyCount = dependencies.size();
		//renderPassInfo.pDependencies = dependencies.data();
		//renderPassInfo.flags = 0;
		//renderPassInfo.pNext = nullptr;

		//ThrowIfFailed(vkCreateRenderPass(device, &renderPassInfo, nullptr, static_cast<VkRenderPass*>(renderPass)));
	}

	void DeviceManagerVK::CreateCommandBuffer(CommandBuffer* commandBuffer) const
	{
		VkCommandBufferAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		allocInfo.commandPool = renderingCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		// DX12 creates command buffers in the recording state so we should do the same here
		VkCommandBuffer internalCommandBuffer;
		{
			if (vkAllocateCommandBuffers(device, &allocInfo, &internalCommandBuffer) != VK_SUCCESS)
			{
				throw("Failed to allocate command buffers!");
			}

			VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
			beginInfo.flags = 0;
			beginInfo.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(internalCommandBuffer, &beginInfo) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to begin recording command buffer!");
			}
		}

		auto vkCommandBuffer = new CommandBufferVK(&internalCommandBuffer);

		commandBuffer = static_cast<CommandBuffer*>(vkCommandBuffer);
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

	void DeviceManagerVK::CreateSwapChain()
	{
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
	}
}