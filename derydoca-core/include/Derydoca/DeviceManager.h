#pragma once

#if USE_DX12
#include <DXGI.h>
#include <d3d12.h>
#endif

namespace DerydocaEngine
{
	enum class RenderingAPI {
		Direct3D12,
		Vulkan
	};

	struct DeviceManagerSettings
	{
	};

	class DeviceManager
	{
	public:
		static DeviceManager* Create(const RenderingAPI renderingAPI);
	};
}