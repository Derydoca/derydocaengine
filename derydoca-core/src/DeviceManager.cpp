#include "Derydoca/DeviceManager.h"
#include "Derydoca/Logging/Log.h"

#include "Derydoca/Logging/Log.h"
#ifdef USE_DX12
#include "Derydoca/Rendering/DX12/DeviceManagerDX12.h"
#endif
#ifdef USE_VULKAN
#include "Derydoca/Rendering/VK/DeviceManagerVK.h"
#endif

namespace Derydoca::Rendering {
    DefaultMessageCallback& DefaultMessageCallback::GetInstance()
    {
        static DefaultMessageCallback Instance;
        return Instance;
    }

    void DefaultMessageCallback::message(nvrhi::MessageSeverity severity, const char* messageText)
    {
        switch (severity)
        {
        case nvrhi::MessageSeverity::Fatal:
            D_LOG_CRITICAL("GAPI Fatal: {0}", messageText);
            break;
        case nvrhi::MessageSeverity::Error:
            D_LOG_ERROR("GAPI Error: {0}", messageText);
            break;
        case nvrhi::MessageSeverity::Warning:
            D_LOG_WARN("GAPI Warning: {0}", messageText);
            break;
        case nvrhi::MessageSeverity::Info:
            D_LOG_INFO("GAPI Info: {0}", messageText);
            break;
        default:
            D_LOG_ERROR("GAPI Unkown Severity: {0}", messageText);
            break;
        }
    }

    DeviceManager* DeviceManager::Create(const RenderingAPI renderingAPI, const DeviceManagerSettings& settings, SDL_Window* sdlWindow)
    {
        DeviceManager* deviceManager = nullptr;
        switch (renderingAPI)
        {
#ifdef USE_DX12
        case RenderingAPI::Direct3D12:
        {
            deviceManager = new Rendering::DeviceManagerDX12(settings, sdlWindow);
            break;
        }
#endif
#ifdef USE_VULKAN
        case RenderingAPI::Vulkan:
        {
            deviceManager = new Rendering::DeviceManagerVK(settings, sdlWindow);
            break;
        }
#endif
        default:
        {
            return deviceManager;
            break;
        }
        }

        return deviceManager;
    }

    void DeviceManager::SignalWindowResizedEvent()
    {
        framebufferResized = true;
    }
}
