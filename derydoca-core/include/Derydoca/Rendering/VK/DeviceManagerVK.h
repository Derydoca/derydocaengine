#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

#include "Derydoca/DeviceManager.h"
#include "Derydoca/Rendering/Common.h"

namespace Derydoca::Rendering
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool IsComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class DeviceManagerVK : public DeviceManager
	{
	public:
		DeviceManagerVK() = default;
		DeviceManagerVK(const DeviceManagerVK&) = delete;
		~DeviceManagerVK() = default;

		void Render() override;
		void CreateRenderPass(const RenderPassDesc& renderPassDesc, RenderPass* renderPass) override;
		void CreateCommandBuffer(CommandBuffer* commandBuffer) const override;

	protected:
		bool CreateDeviceAndSwapChain() override;
		void DestroyDeviceAndSwapChain() override;
		nvrhi::GraphicsAPI GetGraphicsAPI() const override;
		uint32_t GetBackBufferCount() override;
		void ResizeSwapChain() override;
		nvrhi::ITexture* GetBackBuffer(uint32_t index) override;

	private:
		DIRECT_ENUM_TRANSLATE_FUNCS(ImageSampleCount, VkSampleCountFlagBits);
		DIRECT_ENUM_TRANSLATE_FUNCS(ImageLayout, VkImageLayout);
		DIRECT_ENUM_TRANSLATE_FUNCS(PipelineBindPoint, VkPipelineBindPoint);
		DIRECT_ENUM_TRANSLATE_FUNCS(DependencyFlags, VkDependencyFlags);
		DIRECT_ENUM_TRANSLATE_FUNC_TO(AccessFlags, VkAccessFlags);
		DIRECT_ENUM_TRANSLATE_FUNC_TO(PipelineStageFlags, VkPipelineStageFlags);

		ImageFormat Translate(const VkFormat format);
		VkAttachmentLoadOp Translate(const RenderPassBeginningAccess access);
		VkAttachmentStoreOp Translate(const RenderPassEndingAccess access);
		VkAttachmentReference Translate(AttachmentReference attachment);
		inline VkRenderPass* Translate(RenderPass* renderPass) { return static_cast<VkRenderPass*>(renderPass); }

		bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		bool DeviceIsSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		QueueFamilyIndices FindDeviceQueues(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats, VkFormat preferredFormat, VkColorSpaceKHR preferredColorSpace);
		VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		void CreateSwapChain();
		void CreateImageViews();
		void CreateFramebuffers();

		void RecreateSwapChain();
		void CleanupSwapChain();

		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		VkRenderPass mainRenderPass;
		VkCommandPool renderingCommandPool; // TODO: Create instance for each frame in-flight & thread?
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		uint32_t currentFrame = 0;

		VkAllocationCallbacks* allocationCallbacks = nullptr;

		const std::vector<const char*> deviceExtensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

	};
}
