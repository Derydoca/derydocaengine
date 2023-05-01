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

    static const struct
    {
        nvrhi::Format format;
        uint32_t redBits;
        uint32_t greenBits;
        uint32_t blueBits;
        uint32_t alphaBits;
        uint32_t depthBits;
        uint32_t stencilBits;
    } formatInfo[] = {
        { nvrhi::Format::UNKNOWN,            0,  0,  0,  0,  0,  0, },
        { nvrhi::Format::R8_UINT,            8,  0,  0,  0,  0,  0, },
        { nvrhi::Format::RG8_UINT,           8,  8,  0,  0,  0,  0, },
        { nvrhi::Format::RG8_UNORM,          8,  8,  0,  0,  0,  0, },
        { nvrhi::Format::R16_UINT,          16,  0,  0,  0,  0,  0, },
        { nvrhi::Format::R16_UNORM,         16,  0,  0,  0,  0,  0, },
        { nvrhi::Format::R16_FLOAT,         16,  0,  0,  0,  0,  0, },
        { nvrhi::Format::RGBA8_UNORM,        8,  8,  8,  8,  0,  0, },
        { nvrhi::Format::RGBA8_SNORM,        8,  8,  8,  8,  0,  0, },
        { nvrhi::Format::BGRA8_UNORM,        8,  8,  8,  8,  0,  0, },
        { nvrhi::Format::SRGBA8_UNORM,       8,  8,  8,  8,  0,  0, },
        { nvrhi::Format::SBGRA8_UNORM,       8,  8,  8,  8,  0,  0, },
        { nvrhi::Format::R10G10B10A2_UNORM, 10, 10, 10,  2,  0,  0, },
        { nvrhi::Format::R11G11B10_FLOAT,   11, 11, 10,  0,  0,  0, },
        { nvrhi::Format::RG16_UINT,         16, 16,  0,  0,  0,  0, },
        { nvrhi::Format::RG16_FLOAT,        16, 16,  0,  0,  0,  0, },
        { nvrhi::Format::R32_UINT,          32,  0,  0,  0,  0,  0, },
        { nvrhi::Format::R32_FLOAT,         32,  0,  0,  0,  0,  0, },
        { nvrhi::Format::RGBA16_FLOAT,      16, 16, 16, 16,  0,  0, },
        { nvrhi::Format::RGBA16_UNORM,      16, 16, 16, 16,  0,  0, },
        { nvrhi::Format::RGBA16_SNORM,      16, 16, 16, 16,  0,  0, },
        { nvrhi::Format::RG32_UINT,         32, 32,  0,  0,  0,  0, },
        { nvrhi::Format::RG32_FLOAT,        32, 32,  0,  0,  0,  0, },
        { nvrhi::Format::RGB32_UINT,        32, 32, 32,  0,  0,  0, },
        { nvrhi::Format::RGB32_FLOAT,       32, 32, 32,  0,  0,  0, },
        { nvrhi::Format::RGBA32_UINT,       32, 32, 32, 32,  0,  0, },
        { nvrhi::Format::RGBA32_FLOAT,      32, 32, 32, 32,  0,  0, },
    };

    DefaultMessageCallback& DefaultMessageCallback::GetInstance()
    {
        static DefaultMessageCallback Instance;
        return Instance;
    }

    DeviceManager::DeviceManager()
    {
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

    DeviceManager* DeviceManager::Create(const RenderingAPI renderingAPI)
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

        return deviceManager;
    }

    void DeviceManager::UpdateWindowSize()
    {
        int width;
        int height;
        glfwGetWindowSize(m_Window, &width, &height);

        if (width == 0 || height == 0)
        {
            // window is minimized
            m_windowVisible = false;
            return;
        }

        m_windowVisible = true;

        if (int(m_DeviceParams.width) != width ||
            int(m_DeviceParams.height) != height ||
            (m_DeviceParams.vsyncEnabled != m_RequestedVSync && GetGraphicsAPI() == nvrhi::GraphicsAPI::VULKAN))
        {
            // window is not minimized, and the size has changed

            BackBufferResizing();

            m_DeviceParams.width = width;
            m_DeviceParams.height = height;
            m_DeviceParams.vsyncEnabled = m_RequestedVSync;

            ResizeSwapChain();
            BackBufferResized();
        }

        m_DeviceParams.vsyncEnabled = m_RequestedVSync;
    }

    bool DeviceManager::CreateWindowDeviceAndSwapChain(const DeviceCreationParams& params, const char* windowTitle)
    {
        // TODO: Per-monitor DPI for Windows

        if (!glfwInit())
        {
            return false;
        }

        // TODO: VSync, GLFW error callback

        glfwDefaultWindowHints();


        bool foundFormat = false;
        for (const auto& info : formatInfo)
        {
            if (info.format == params.swapChainFormat)
            {
                glfwWindowHint(GLFW_RED_BITS, info.redBits);
                glfwWindowHint(GLFW_GREEN_BITS, info.greenBits);
                glfwWindowHint(GLFW_BLUE_BITS, info.blueBits);
                glfwWindowHint(GLFW_ALPHA_BITS, info.alphaBits);
                glfwWindowHint(GLFW_DEPTH_BITS, info.depthBits);
                glfwWindowHint(GLFW_STENCIL_BITS, info.stencilBits);
                foundFormat = true;
                break;
            }
        }

        assert(foundFormat);

        glfwWindowHint(GLFW_SAMPLES, params.sampleCount);
        glfwWindowHint(GLFW_REFRESH_RATE, params.refreshRate);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);   // Ignored for fullscreen

        m_Window = glfwCreateWindow(params.width, params.height,
            windowTitle ? windowTitle : "",
            params.startFullscreen ? glfwGetPrimaryMonitor() : nullptr,
            nullptr);

        if (m_Window == nullptr)
        {
            return false;
        }

        if (params.startFullscreen)
        {
            glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0,
                m_DeviceParams.width, m_DeviceParams.height, m_DeviceParams.refreshRate);
        }
        else
        {
            int fbWidth = 0, fbHeight = 0;
            glfwGetFramebufferSize(m_Window, &fbWidth, &fbHeight);
            m_DeviceParams.width = fbWidth;
            m_DeviceParams.height = fbHeight;
        }

        if (windowTitle)
        {
            m_WindowTitle = windowTitle;
        }

        glfwSetWindowUserPointer(m_Window, this);

        if (params.windowPosX != -1 && params.windowPosY != -1)
        {
            glfwSetWindowPos(m_Window, params.windowPosX, params.windowPosY);
        }

        if (params.startMaximized)
        {
            glfwMaximizeWindow(m_Window);
        }

        //glfwSetWindowPosCallback(m_Window, WindowPosCallback_GLFW);
        //glfwSetWindowCloseCallback(m_Window, WindowCloseCallback_GLFW);
        //glfwSetWindowRefreshCallback(m_Window, WindowRefreshCallback_GLFW);
        //glfwSetWindowFocusCallback(m_Window, WindowFocusCallback_GLFW);
        //glfwSetWindowIconifyCallback(m_Window, WindowIconifyCallback_GLFW);
        //glfwSetKeyCallback(m_Window, KeyCallback_GLFW);
        //glfwSetCharModsCallback(m_Window, CharModsCallback_GLFW);
        //glfwSetCursorPosCallback(m_Window, MousePosCallback_GLFW);
        //glfwSetMouseButtonCallback(m_Window, MouseButtonCallback_GLFW);
        //glfwSetScrollCallback(m_Window, MouseScrollCallback_GLFW);
        //glfwSetJoystickCallback(JoystickConnectionCallback_GLFW);

        //// If there are multiple device managers, then this would be called by each one which isn't necessary
        //// but should not hurt.
        //JoyStickManager::Singleton().EnumerateJoysticks();

        if (!CreateDeviceAndSwapChain())
        {
            return false;
        }

        glfwShowWindow(m_Window);

        // reset the back buffer size state to enforce a resize event
        m_DeviceParams.width = 0;
        m_DeviceParams.height = 0;

        UpdateWindowSize();


        return true;
    }

    void DeviceManager::BackBufferResizing()
    {
        m_SwapChainFramebuffers.clear();

        for (auto it : m_vApplicationLayers)
        {
            it->BackBufferResizing();
        }
    }

    void DeviceManager::BackBufferResized()
    {
        for (auto it : m_vApplicationLayers)
        {
            it->BackBufferResized(m_DeviceParams.width,
                m_DeviceParams.height,
                m_DeviceParams.sampleCount);
        }

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
        return m_NvrhiDevice;
    }

    void DeviceManager::Shutdown()
    {
        m_SwapChainFramebuffers.clear();

        DestroyDeviceAndSwapChain();

        if (m_Window)
        {
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;
        }

        glfwTerminate();
    }

    void DeviceManager::SignalWindowResizedEvent()
    {
        framebufferResized = true;
    }
}
