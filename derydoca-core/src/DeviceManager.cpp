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

    DeviceManager::DeviceManager(const DeviceManagerSettings& deviceSettings, SDL_Window* sdlWindow)
    {
        m_DeviceSettings = deviceSettings;
        window = sdlWindow;

        m_RequestedVSync = m_DeviceSettings.vsyncEnabled;
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

    void DeviceManager::UpdateWindowSize()
    {
        int width;
        int height;
        SDL_GetWindowSize(window, &width, &height);

        if (width == 0 || height == 0)
        {
            // window is minimized
            m_windowVisible = false;
            return;
        }

        m_windowVisible = true;

        if (int(m_DeviceSettings.width) != width ||
            int(m_DeviceSettings.height) != height ||
            (m_DeviceSettings.vsyncEnabled != m_RequestedVSync && GetGraphicsAPI() == nvrhi::GraphicsAPI::VULKAN))
        {
            // window is not minimized, and the size has changed

            BackBufferResizing();

            m_DeviceSettings.width = width;
            m_DeviceSettings.height = height;
            m_DeviceSettings.vsyncEnabled = m_RequestedVSync;

            ResizeSwapChain();
            BackBufferResized();
        }

        //m_DeviceParams.vsyncEnabled = m_RequestedVSync;
    }

    void DeviceManager::BackBufferResizing()
    {
        m_SwapChainFramebuffers.clear();

        //for (auto it : m_vRenderPasses)
        //{
        //    it->BackBufferResizing();
        //}
    }

    void DeviceManager::BackBufferResized()
    {
        //for (auto it : m_vRenderPasses)
        //{
        //    it->BackBufferResized(m_DeviceParams.backBufferWidth,
        //        m_DeviceParams.backBufferHeight,
        //        m_DeviceParams.swapChainSampleCount);
        //}

        uint32_t backBufferCount = GetBackBufferCount();
        m_SwapChainFramebuffers.resize(backBufferCount);
        for (uint32_t index = 0; index < backBufferCount; index++)
        {
            m_SwapChainFramebuffers[index] = GetDevice()->createFramebuffer(
                nvrhi::FramebufferDesc().addColorAttachment(GetBackBuffer(index)));
        }
    }

    nvrhi::IDevice* DeviceManager::GetDevice()
    {
        return m_nvrhiDevice;
    }

    void DeviceManager::SignalWindowResizedEvent()
    {
        framebufferResized = true;
    }
}
