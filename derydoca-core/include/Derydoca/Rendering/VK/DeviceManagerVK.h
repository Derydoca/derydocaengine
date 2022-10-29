#pragma once

#include "Derydoca/DeviceManager.h"

namespace DerydocaEngine::Rendering
{
	class DeviceManagerVK : public DeviceManager
	{
	public:
		DeviceManagerVK() = default;
		DeviceManagerVK(const DeviceManagerVK&) = delete;

		void FindLogicalDevice() override;
	};
}
