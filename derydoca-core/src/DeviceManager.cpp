#include "Derydoca/DeviceManager.h"
#include "Derydoca/Logging/Log.h"

#include "Derydoca/Logging/Log.h"
#ifdef USE_DX12
#include "Derydoca/Rendering/DX12/DeviceManagerDX12.h"
#endif
#ifdef USE_VULKAN
#include "Derydoca/Rendering/VK/DeviceManagerVK.h"
#endif

namespace DerydocaEngine {
	DeviceManager* DerydocaEngine::DeviceManager::Create(const RenderingAPI renderingAPI)
	{

		DeviceManager* deviceManager = nullptr;
		switch (renderingAPI)
		{
#ifdef USE_DX12
		case RenderingAPI::Direct3D12:
		{
			deviceManager = new Rendering::DeviceManagerDX12();
			break;
		}
#endif
#ifdef USE_VULKAN
		case RenderingAPI::Vulkan:
		{
			deviceManager = new Rendering::DeviceManagerVK();
			break;
		}
#endif
		default:
		{
			return deviceManager;
			break;
		}
		}

		deviceManager->FindLogicalDevice();
	}
}
