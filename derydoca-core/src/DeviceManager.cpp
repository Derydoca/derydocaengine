#include "Derydoca/DeviceManager.h"
#include "Derydoca/Logging/Log.h"

#include "Derydoca/Logging/Log.h"
#ifdef USE_DX12
#include "Derydoca/Rendering/DX12/DeviceManagerDX12.h"
#endif
#ifdef USE_VULKAN
#include "Derydoca/Rendering/VK/DeviceManagerVK.h"
#endif

#ifdef D_WIN32
#include <ShellScalingApi.h>
#pragma comment(lib, "shcore.lib")
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

    static void WindowIconifyCallback_GLFW(GLFWwindow* window, int iconified)
    {
        DeviceManager* manager = reinterpret_cast<DeviceManager*>(glfwGetWindowUserPointer(window));
        manager->WindowIconifyCallback(iconified);
    }

    static void WindowFocusCallback_GLFW(GLFWwindow* window, int focused)
    {
        DeviceManager* manager = reinterpret_cast<DeviceManager*>(glfwGetWindowUserPointer(window));
        manager->WindowFocusCallback(focused);
    }

    static void WindowRefreshCallback_GLFW(GLFWwindow* window)
    {
        DeviceManager* manager = reinterpret_cast<DeviceManager*>(glfwGetWindowUserPointer(window));
        manager->WindowRefreshCallback();
    }

    static void WindowCloseCallback_GLFW(GLFWwindow* window)
    {
        DeviceManager* manager = reinterpret_cast<DeviceManager*>(glfwGetWindowUserPointer(window));
        manager->WindowCloseCallback();
    }

    static void WindowPosCallback_GLFW(GLFWwindow* window, int xpos, int ypos)
    {
        DeviceManager* manager = reinterpret_cast<DeviceManager*>(glfwGetWindowUserPointer(window));
        manager->WindowPosCallback(xpos, ypos);
    }

    static void KeyCallback_GLFW(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        DeviceManager* manager = reinterpret_cast<DeviceManager*>(glfwGetWindowUserPointer(window));
        manager->KeyboardUpdate(key, scancode, action, mods);
    }

    static void CharModsCallback_GLFW(GLFWwindow* window, unsigned int unicode, int mods)
    {
        DeviceManager* manager = reinterpret_cast<DeviceManager*>(glfwGetWindowUserPointer(window));
        manager->KeyboardCharInput(unicode, mods);
    }

    static void MousePosCallback_GLFW(GLFWwindow* window, double xpos, double ypos)
    {
        DeviceManager* manager = reinterpret_cast<DeviceManager*>(glfwGetWindowUserPointer(window));
        manager->MousePosUpdate(xpos, ypos);
    }

    static void MouseButtonCallback_GLFW(GLFWwindow* window, int button, int action, int mods)
    {
        DeviceManager* manager = reinterpret_cast<DeviceManager*>(glfwGetWindowUserPointer(window));
        manager->MouseButtonUpdate(button, action, mods);
    }

    static void MouseScrollCallback_GLFW(GLFWwindow* window, double xoffset, double yoffset)
    {
        DeviceManager* manager = reinterpret_cast<DeviceManager*>(glfwGetWindowUserPointer(window));
        manager->MouseScrollUpdate(xoffset, yoffset);
    }

    static void JoystickConnectionCallback_GLFW(int joyId, int connectDisconnect)
    {
        // TODO: Joystick support
        if (connectDisconnect == GLFW_CONNECTED)
        {
            D_LOG_INFO("Joystick connected.");
            //JoyStickManager::Singleton().ConnectJoystick(joyId);
        }
        if (connectDisconnect == GLFW_DISCONNECTED)
        {
            D_LOG_INFO("Joystick disconnected.");
            //JoyStickManager::Singleton().DisconnectJoystick(joyId);
        }
    }

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
    
    void DeviceManager::AddRenderPassToFront(IRenderPass* pRenderPass)
    {
        m_vRenderPasses.remove(pRenderPass);
        m_vRenderPasses.push_front(pRenderPass);

        pRenderPass->BackBufferResizing();
        pRenderPass->BackBufferResized(
            m_DeviceParams.backBufferWidth,
            m_DeviceParams.backBufferHeight,
            m_DeviceParams.swapChainSampleCount);
    }
    
    void DeviceManager::AddRenderPassToBack(IRenderPass* pRenderPass)
    {
        m_vRenderPasses.remove(pRenderPass);
        m_vRenderPasses.push_back(pRenderPass);

        pRenderPass->BackBufferResizing();
        pRenderPass->BackBufferResized(
            m_DeviceParams.backBufferWidth,
            m_DeviceParams.backBufferHeight,
            m_DeviceParams.swapChainSampleCount);
    }

    void DeviceManager::RemoveRenderPass(IRenderPass* pRenderPass)
    {
        m_vRenderPasses.remove(pRenderPass);
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

        if (int(m_DeviceParams.backBufferWidth) != width ||
            int(m_DeviceParams.backBufferHeight) != height ||
            (m_DeviceParams.vsyncEnabled != m_RequestedVSync && GetGraphicsAPI() == nvrhi::GraphicsAPI::VULKAN))
        {
            // window is not minimized, and the size has changed

            BackBufferResizing();

            m_DeviceParams.backBufferWidth = width;
            m_DeviceParams.backBufferHeight = height;
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

        glfwWindowHint(GLFW_SAMPLES, params.swapChainSampleCount);
        glfwWindowHint(GLFW_REFRESH_RATE, params.refreshRate);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);   // Ignored for fullscreen

        m_Window = glfwCreateWindow(params.backBufferWidth, params.backBufferHeight,
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
                m_DeviceParams.backBufferWidth, m_DeviceParams.backBufferHeight, m_DeviceParams.refreshRate);
        }
        else
        {
            int fbWidth = 0, fbHeight = 0;
            glfwGetFramebufferSize(m_Window, &fbWidth, &fbHeight);
            m_DeviceParams.backBufferWidth = fbWidth;
            m_DeviceParams.backBufferHeight = fbHeight;
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

        glfwSetWindowPosCallback(m_Window, WindowPosCallback_GLFW);
        glfwSetWindowCloseCallback(m_Window, WindowCloseCallback_GLFW);
        glfwSetWindowRefreshCallback(m_Window, WindowRefreshCallback_GLFW);
        glfwSetWindowFocusCallback(m_Window, WindowFocusCallback_GLFW);
        glfwSetWindowIconifyCallback(m_Window, WindowIconifyCallback_GLFW);
        glfwSetKeyCallback(m_Window, KeyCallback_GLFW);
        glfwSetCharModsCallback(m_Window, CharModsCallback_GLFW);
        glfwSetCursorPosCallback(m_Window, MousePosCallback_GLFW);
        glfwSetMouseButtonCallback(m_Window, MouseButtonCallback_GLFW);
        glfwSetScrollCallback(m_Window, MouseScrollCallback_GLFW);
        glfwSetJoystickCallback(JoystickConnectionCallback_GLFW);

        // TODO: Joystick support
        //// If there are multiple device managers, then this would be called by each one which isn't necessary
        //// but should not hurt.
        //JoyStickManager::Singleton().EnumerateJoysticks();

        if (!CreateDeviceAndSwapChain())
        {
            return false;
        }

        glfwShowWindow(m_Window);

        // reset the back buffer size state to enforce a resize event
        m_DeviceParams.backBufferWidth = 0;
        m_DeviceParams.backBufferHeight = 0;

        UpdateWindowSize();


        return true;
    }
    
    void DeviceManager::RunMessageLoop()
    {
        m_PreviousFrameTimestamp = glfwGetTime();

        while (!glfwWindowShouldClose(m_Window))
        {

            //if (m_callbacks.beforeFrame) m_callbacks.beforeFrame(*this);

            glfwPollEvents();
            UpdateWindowSize();

            double curTime = glfwGetTime();
            double elapsedTime = curTime - m_PreviousFrameTimestamp;

            //JoyStickManager::Singleton().EraseDisconnectedJoysticks();
            //JoyStickManager::Singleton().UpdateAllJoysticks(m_vRenderPasses);

            //if (m_windowVisible)
            //{
            //    if (m_callbacks.beforeAnimate) m_callbacks.beforeAnimate(*this);
            //    Animate(elapsedTime);
            //    if (m_callbacks.afterAnimate) m_callbacks.afterAnimate(*this);
            //    if (m_callbacks.beforeRender) m_callbacks.beforeRender(*this);
            //    Render();
            //    if (m_callbacks.afterRender) m_callbacks.afterRender(*this);
            //    if (m_callbacks.beforePresent) m_callbacks.beforePresent(*this);
            //    Present();
            //    if (m_callbacks.afterPresent) m_callbacks.afterPresent(*this);
            //}

            std::this_thread::sleep_for(std::chrono::milliseconds(0));

            GetDevice()->runGarbageCollection();

            //UpdateAverageFrameTime(elapsedTime);
            m_PreviousFrameTimestamp = curTime;

            ++m_FrameIndex;

        }

        GetDevice()->waitForIdle();
    }

    void DeviceManager::WindowPosCallback(int xpos, int ypos)
    {
        D_LOG_TRACE("Window position update. [{}, {}]", xpos, ypos);

#ifdef D_WIN32
        if (m_DeviceParams.enablePerMonitorDPI)
        {
            HWND hwnd = glfwGetWin32Window(m_Window);
            auto monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);

            unsigned int dpiX;
            unsigned int dpiY;
            GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);

            m_DPIScaleFactorX = dpiX / 96.f;
            m_DPIScaleFactorY = dpiY / 96.f;
        }
#endif
    }

    void DeviceManager::KeyboardUpdate(int key, int scancode, int action, int mods)
    {
        D_LOG_TRACE("Keyboard uppdate: Key: {}, scancode: {}, action: {}, mods: {}", key, scancode, action, mods);

        if (key == -1)
        {
            // filter unknown keys
            return;
        }

        //for (auto it = m_vRenderPasses.crbegin(); it != m_vRenderPasses.crend(); it++)
        //{
        //    bool ret = (*it)->KeyboardUpdate(key, scancode, action, mods);
        //    if (ret)
        //        break;
        //}
    }

    void DeviceManager::KeyboardCharInput(unsigned int unicode, int mods)
    {
        D_LOG_TRACE("Keyboard character input: Unicode: {}, mods: {}", unicode, mods);

        //for (auto it = m_vRenderPasses.crbegin(); it != m_vRenderPasses.crend(); it++)
        //{
        //    bool ret = (*it)->KeyboardCharInput(unicode, mods);
        //    if (ret)
        //        break;
        //}
    }

    void DeviceManager::MousePosUpdate(double xpos, double ypos)
    {
        D_LOG_TRACE("Mouse position update: [{}, {}]", xpos, ypos);

        xpos /= m_DPIScaleFactorX;
        ypos /= m_DPIScaleFactorY;

        //for (auto it = m_vRenderPasses.crbegin(); it != m_vRenderPasses.crend(); it++)
        //{
        //    bool ret = (*it)->MousePosUpdate(xpos, ypos);
        //    if (ret)
        //        break;
        //}
    }

    void DeviceManager::MouseButtonUpdate(int button, int action, int mods)
    {
        D_LOG_TRACE("Mouse button update: Button: {}, action: {}, mods: {}", button, action, mods);

        //for (auto it = m_vRenderPasses.crbegin(); it != m_vRenderPasses.crend(); it++)
        //{
        //    bool ret = (*it)->MouseButtonUpdate(button, action, mods);
        //    if (ret)
        //        break;
        //}
    }

    void DeviceManager::MouseScrollUpdate(double xoffset, double yoffset)
    {
        D_LOG_TRACE("Mouse scroll update: Xoffset: {}, yoffset: {}", xoffset, yoffset);

        //for (auto it = m_vRenderPasses.crbegin(); it != m_vRenderPasses.crend(); it++)
        //{
        //    bool ret = (*it)->MouseScrollUpdate(xoffset, yoffset);
        //    if (ret)
        //        break;
        //}
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
            it->BackBufferResized(m_DeviceParams.backBufferWidth,
                m_DeviceParams.backBufferHeight,
                m_DeviceParams.swapChainSampleCount);
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
