#pragma once

#include "Derydoca/DeviceManager.h"

namespace DerydocaEngine::Rendering
{
	class DeviceManagerVK : public DeviceManager
	{
	public:
		DeviceManagerVK() = default;
		DeviceManagerVK(const DeviceManagerVK&) = delete;

		void Initialize(const DeviceManagerSettings& settings, SDL_Window* sdlWindow) override;
	};
}
