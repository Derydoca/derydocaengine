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
		switch (renderingAPI)
		{
#ifdef USE_DX12
		case RenderingAPI::Direct3D12:
			return new Rendering::DeviceManagerDX12();
#endif
#ifdef USE_VULKAN
		case RenderingAPI::Vulkan:
			return new Rendering::DeviceManagerVK();
#endif
		default:
			return nullptr;
		}
	}
}
