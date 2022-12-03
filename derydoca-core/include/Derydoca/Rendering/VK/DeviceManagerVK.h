#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

#include "Derydoca/DeviceManager.h"
#include "Derydoca/Rendering/Common.h"
#include "Derydoca/Rendering/VK/RenderPassVK.h"

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
#define DIRECT_ENUM_TRANSLATE_FUNC_TO(GenericType, VkType) inline VkType Translate(GenericType value) { return static_cast<VkType>(value); }
#define DIRECT_ENUM_TRANSLATE_FUNC_FROM(GenericType, VkType) inline GenericType Translate(VkType value) { return static_cast<GenericType>(value); }
#define DIRECT_ENUM_TRANSLATE_FUNCS(GenericType, VkType) DIRECT_ENUM_TRANSLATE_FUNC_TO(GenericType, VkType); DIRECT_ENUM_TRANSLATE_FUNC_FROM(GenericType, VkType)
	public:
		DeviceManagerVK() = default;
		DeviceManagerVK(const DeviceManagerVK&) = delete;

		void Initialize(const DeviceManagerSettings& settings) override;
		void Render() override;
		void Cleanup() override;
		void CreateRenderPass(const RenderPassDesc& renderPassDesc, RenderPass* renderPass) override;

	private:
		VkFormat Translate(const ImageFormat format);
		ImageFormat Translate(const VkFormat format);
		//VkSampleCountFlagBits Translate(const ImageSampleCount sampleCount);
		DIRECT_ENUM_TRANSLATE_FUNCS(ImageSampleCount, VkSampleCountFlagBits);
		VkAttachmentLoadOp Translate(const RenderPassBeginningAccess access);
		VkAttachmentStoreOp Translate(const RenderPassEndingAccess access);
		//VkImageLayout Translate(const ImageLayout layout);
		DIRECT_ENUM_TRANSLATE_FUNCS(ImageLayout, VkImageLayout);
		//VkPipelineBindPoint Translate(const PipelineBindPoint bindPoint);
		DIRECT_ENUM_TRANSLATE_FUNCS(PipelineBindPoint, VkPipelineBindPoint);
		VkAttachmentReference Translate(AttachmentReference attachment);
		//VkDependencyFlags Translate(DependencyFlags flags);
		DIRECT_ENUM_TRANSLATE_FUNCS(DependencyFlags, VkDependencyFlags);
		//VkAccessFlags Translate(AccessFlags flags);
		DIRECT_ENUM_TRANSLATE_FUNC_TO(AccessFlags, VkAccessFlags);
		//VkPipelineStageFlags Translate(PipelineStageFlags flags);
		DIRECT_ENUM_TRANSLATE_FUNC_TO(PipelineStageFlags, VkPipelineStageFlags);
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
		VkExtent2D ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR& capabilities, SDL_Window* window);

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
