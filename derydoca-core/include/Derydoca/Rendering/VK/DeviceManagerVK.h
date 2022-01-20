#pragma once

#include "Derydoca/DeviceManager.h"

namespace DerydocaEngine::Rendering
{
	class DeviceManagerVK : public DeviceManager
	{
	public:
		nvrhi::IDevice* GetDevice() const override
		{
			return NULL;
		}

	protected:
		bool CreateDeviceAndSwapChain() override;
		void DestroyDeviceAndSwapChain() override;
		void Present() override;

	};
}
