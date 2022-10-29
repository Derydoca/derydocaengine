#pragma once

#include <algorithm>

#define NOMINMAX
#include <Windows.h>
#include <dxgi1_5.h>
#include <dxgidebug.h>

#include "Derydoca/DeviceManager.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

namespace DerydocaEngine::Rendering
{
	class DeviceManagerDX12 : public DeviceManager
	{
	public:
		DeviceManagerDX12() = default;
		DeviceManagerDX12(const DeviceManagerDX12&) = delete;

		void FindLogicalDevice() override;

	private:
		void GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter);
	};
}
