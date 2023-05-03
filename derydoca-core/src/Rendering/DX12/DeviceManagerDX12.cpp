#include "Derydoca/Rendering/DX12/DeviceManagerDX12.h"
#include "Derydoca/Logging/Log.h"
#include "Derydoca/Rendering/DX12/DXHelper.h"

#include <string>
#include <fmt/xchar.h>
#include <sstream>

#define D3D_ACTIVE_FEATURE_LEVEL D3D_FEATURE_LEVEL_12_1

namespace Derydoca::Rendering
{
    void MessageCallback(
        D3D12_MESSAGE_CATEGORY Category,
        D3D12_MESSAGE_SEVERITY Severity,
        D3D12_MESSAGE_ID ID,
        LPCSTR pDescription,
        void* pContext)
    {
        std::string categoryStr;
        switch (Category)
        {
        case D3D12_MESSAGE_CATEGORY_APPLICATION_DEFINED:
            categoryStr = "Application Defined";
            break;
        case D3D12_MESSAGE_CATEGORY_MISCELLANEOUS:
            categoryStr = "Miscellaneous";
            break;
        case D3D12_MESSAGE_CATEGORY_INITIALIZATION:
            categoryStr = "Initialization";
            break;
        case D3D12_MESSAGE_CATEGORY_CLEANUP:
            categoryStr = "Cleanup";
            break;
        case D3D12_MESSAGE_CATEGORY_COMPILATION:
            categoryStr = "Compilation";
            break;
        case D3D12_MESSAGE_CATEGORY_STATE_CREATION:
            categoryStr = "State Creation";
            break;
        case D3D12_MESSAGE_CATEGORY_STATE_SETTING:
            categoryStr = "State Setting";
            break;
        case D3D12_MESSAGE_CATEGORY_STATE_GETTING:
            categoryStr = "State Getting";
            break;
        case D3D12_MESSAGE_CATEGORY_RESOURCE_MANIPULATION:
            categoryStr = "Resource Manipulation";
            break;
        case D3D12_MESSAGE_CATEGORY_EXECUTION:
            categoryStr = "Execution";
            break;
        case D3D12_MESSAGE_CATEGORY_SHADER:
            categoryStr = "Shader";
            break;
        default:
            categoryStr = "Unknown";
            break;
        }

        switch (Severity)
        {
        case D3D12_MESSAGE_SEVERITY_CORRUPTION:
            D_LOG_CRITICAL("D3D12 Corruption: {0}\n    Message ID: {1} [{1:x}]\n    Category:   {2}", pDescription, ID, categoryStr);
            break;
        case D3D12_MESSAGE_SEVERITY_ERROR:
            D_LOG_ERROR("D3D12 Error: {0}\n    Message ID: {1} [{1:x}]\n    Category:   {2}", pDescription, ID, categoryStr);
            break;
        case D3D12_MESSAGE_SEVERITY_WARNING:
            D_LOG_WARN("D3D12 Warning: {0}\n    Message ID: {1} [{1:x}]\n    Category:   {2}", pDescription, ID, categoryStr);
            break;
        case D3D12_MESSAGE_SEVERITY_INFO:
            D_LOG_INFO("D3D12 Info: {0}\n    Message ID: {1} [{1:x}]\n    Category:   {2}", pDescription, ID, categoryStr);
            break;
        case D3D12_MESSAGE_SEVERITY_MESSAGE:
            D_LOG_TRACE("D3D12 Message: {0}\n    Message ID: {1} [{1:x}]\n    Category:   {2}", pDescription, ID, categoryStr);
            break;
        default:
            D_LOG_ERROR("D3D12 Unknown Severity Message: {0}\n    Message ID: {1} [{1:x}]\n    Category:   {2}", pDescription, ID, categoryStr);
            break;
        }
    }

    bool DeviceManagerDX12::CreateRenderTargets()
    {
        m_SwapChainBuffers.resize(m_SwapChainDesc.BufferCount);
        m_RhiSwapChainBuffers.resize(m_SwapChainDesc.BufferCount);

        for (UINT n = 0; n < m_SwapChainDesc.BufferCount; n++)
        {
            const HRESULT hr = m_SwapChain->GetBuffer(n, IID_PPV_ARGS(&m_SwapChainBuffers[n]));
            HR_RETURN(hr);

            nvrhi::TextureDesc textureDesc;
            textureDesc.width = m_DeviceParams.backBufferWidth;
            textureDesc.height = m_DeviceParams.backBufferHeight;
            textureDesc.sampleCount = m_DeviceParams.swapChainSampleCount;
            textureDesc.sampleQuality = m_DeviceParams.sampleQuality;
            textureDesc.format = m_DeviceParams.imageFormat;
            textureDesc.debugName = "SwapChainBuffer";
            textureDesc.isRenderTarget = true;
            textureDesc.isUAV = false;
            textureDesc.initialState = nvrhi::ResourceStates::Present;
            textureDesc.keepInitialState = true;

            m_RhiSwapChainBuffers[n] = m_NvrhiDevice->createHandleForNativeTexture(nvrhi::ObjectTypes::D3D12_Resource, nvrhi::Object(m_SwapChainBuffers[n]), textureDesc);
        }

        return true;
    }

    void DeviceManagerDX12::ReleaseRenderTargets()
    {
        // Make sure that all frames have finished rendering
        m_NvrhiDevice->waitForIdle();

        // Release all in-flight references to the render targets
        m_NvrhiDevice->runGarbageCollection();

        // Set the events so that WaitForSingleObject in OneFrame will not hang later
        for (auto e : m_FrameFenceEvents)
            SetEvent(e);

        // Release the old buffers because ResizeBuffers requires that
        m_RhiSwapChainBuffers.clear();
        m_SwapChainBuffers.clear();
    }

    bool DeviceManagerDX12::MoveWindowOntoAdapter(IDXGIAdapter* targetAdapter, RECT& rect)
    {
        assert(targetAdapter != NULL);

        HRESULT hres = S_OK;
        unsigned int outputNo = 0;
        while (SUCCEEDED(hres))
        {
            IDXGIOutput* pOutput = nullptr;
            hres = targetAdapter->EnumOutputs(outputNo++, &pOutput);

            if (SUCCEEDED(hres) && pOutput)
            {
                DXGI_OUTPUT_DESC OutputDesc;
                pOutput->GetDesc(&OutputDesc);
                const RECT desktop = OutputDesc.DesktopCoordinates;
                const int centreX = (int)desktop.left + (int)(desktop.right - desktop.left) / 2;
                const int centreY = (int)desktop.top + (int)(desktop.bottom - desktop.top) / 2;
                const int winW = rect.right - rect.left;
                const int winH = rect.bottom - rect.top;
                const int left = centreX - winW / 2;
                const int right = left + winW;
                const int top = centreY - winH / 2;
                const int bottom = top + winH;
                rect.left = std::max(left, (int)desktop.left);
                rect.right = std::min(right, (int)desktop.right);
                rect.bottom = std::min(bottom, (int)desktop.bottom);
                rect.top = std::max(top, (int)desktop.top);

                // If there is more than one output, go with the first found.  Multi-monitor support could go here.
                return true;
            }
        }

        return false;
    }

    bool DeviceManagerDX12::CreateDeviceAndSwapChain()
    {
        UINT windowStyle = m_DeviceParams.startFullscreen
            ? (WS_POPUP | WS_SYSMENU | WS_VISIBLE)
            : m_DeviceParams.startMaximized
            ? (WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE)
            : (WS_OVERLAPPEDWINDOW | WS_VISIBLE);

        RECT rect = { 0, 0, LONG(m_DeviceParams.backBufferWidth), LONG(m_DeviceParams.backBufferHeight) };
        AdjustWindowRect(&rect, windowStyle, FALSE);

        RefCountPtr<IDXGIAdapter> targetAdapter;
        // TODO: Allow m_DeviceParams to hold onto an adapter as an override
        {
            targetAdapter = GetHardwareAdapter(m_DeviceParams.adapterNameSubstring);

            if (!targetAdapter)
            {
                std::wstring adapterNameStr(m_DeviceParams.adapterNameSubstring.begin(), m_DeviceParams.adapterNameSubstring.end());

                D_LOG_CRITICAL("Unable to find adapter with the name filter of '{0}'.", "adapterNameStr");
                return false;
            }
        }

        {
            DXGI_ADAPTER_DESC aDesc;
            targetAdapter->GetDesc(&aDesc);

            std::wstring adapterName = aDesc.Description;

            std::stringstream ss;
            std::wstringstream wss;
            for (auto c : adapterName)
            {
                ss << wss.narrow(c, '?');
            }
            m_RendererString = ss.str();
        }

        if (MoveWindowOntoAdapter(targetAdapter, rect))
        {
            glfwSetWindowPos(m_Window, rect.left, rect.top);
        }

        m_hWnd = glfwGetWin32Window(m_Window);

        HRESULT hr = E_FAIL;

        RECT clientRect;
        GetClientRect(m_hWnd, &clientRect);
        UINT width = clientRect.right - clientRect.left;
        UINT height = clientRect.bottom - clientRect.top;

        ZeroMemory(&m_SwapChainDesc, sizeof(m_SwapChainDesc));
        m_SwapChainDesc.Width = width;
        m_SwapChainDesc.Height = height;
        m_SwapChainDesc.SampleDesc.Count = m_DeviceParams.swapChainSampleCount;
        m_SwapChainDesc.SampleDesc.Quality = 0;
        m_SwapChainDesc.BufferUsage = m_DeviceParams.swapChainUsage;
        m_SwapChainDesc.BufferCount = m_DeviceParams.bufferCount;
        m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        m_SwapChainDesc.Flags = m_DeviceParams.allowModeSwitch ? DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH : 0;

        // Special processing for sRGB swap chain formats.
        // DXGI will not create a swap chain with an sRGB format, but its contents will be interpreted as sRGB.
        // So we need to use a non-sRGB format here, but store the true sRGB format for later framebuffer creation.
        switch (m_DeviceParams.imageFormat)  // NOLINT(clang-diagnostic-switch-enum)
        {
        case nvrhi::Format::SRGBA8_UNORM:
            m_SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        case nvrhi::Format::SBGRA8_UNORM:
            m_SwapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            break;
        default:
            m_SwapChainDesc.Format = nvrhi::d3d12::convertFormat(m_DeviceParams.imageFormat);
            break;
        }

        if (m_DeviceParams.enableDebugRuntime)
        {
            RefCountPtr<ID3D12Debug> pDebug;
            hr = D3D12GetDebugInterface(IID_PPV_ARGS(&pDebug));
            HR_RETURN(hr);

            pDebug->EnableDebugLayer();
        }

        RefCountPtr<IDXGIFactory2> pDxgiFactory;
        UINT dxgiFactoryFlags = m_DeviceParams.enableDebugRuntime ? DXGI_CREATE_FACTORY_DEBUG : 0;
        hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&pDxgiFactory));
        HR_RETURN(hr);

        RefCountPtr<IDXGIFactory5> pDxgiFactory5;
        if (SUCCEEDED(pDxgiFactory->QueryInterface(IID_PPV_ARGS(&pDxgiFactory5))))
        {
            BOOL supported = 0;
            if (SUCCEEDED(pDxgiFactory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &supported, sizeof(supported))))
            {
                m_TearingSupported = (supported != 0);
            }
        }

        if (m_TearingSupported)
        {
            m_SwapChainDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
        }

        hr = D3D12CreateDevice(
            targetAdapter,
            m_DeviceParams.featureLevel,
            IID_PPV_ARGS(&m_Device12));
        HR_RETURN(hr);

        if (m_DeviceParams.enableDebugRuntime)
        {
            RefCountPtr<ID3D12InfoQueue> pInfoQueue;
            m_Device12->QueryInterface(&pInfoQueue);

            if (pInfoQueue)
            {
#ifdef _DEBUG
                pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
                pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
#endif

                D3D12_MESSAGE_ID disableMessageIDs[] = {
                    D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
                    D3D12_MESSAGE_ID_COMMAND_LIST_STATIC_DESCRIPTOR_RESOURCE_DIMENSION_MISMATCH, // descriptor validation doesn't understand acceleration structures
                };

                D3D12_INFO_QUEUE_FILTER filter = {};
                filter.DenyList.pIDList = disableMessageIDs;
                filter.DenyList.NumIDs = sizeof(disableMessageIDs) / sizeof(disableMessageIDs[0]);
                pInfoQueue->AddStorageFilterEntries(&filter);
            }
        }

        m_DxgiAdapter = targetAdapter;

        D3D12_COMMAND_QUEUE_DESC queueDesc;
        ZeroMemory(&queueDesc, sizeof(queueDesc));
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queueDesc.NodeMask = 1;
        hr = m_Device12->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_GraphicsQueue));
        HR_RETURN(hr);
        m_GraphicsQueue->SetName(L"Graphics Queue");

        if (m_DeviceParams.enableComputeQueue)
        {
            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
            hr = m_Device12->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_ComputeQueue));
            HR_RETURN(hr);
            m_ComputeQueue->SetName(L"Compute Queue");
        }

        if (m_DeviceParams.enableCopyQueue)
        {
            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
            hr = m_Device12->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CopyQueue));
            HR_RETURN(hr);
            m_CopyQueue->SetName(L"Copy Queue");
        }

        m_FullScreenDesc = {};
        m_FullScreenDesc.RefreshRate.Numerator = m_DeviceParams.refreshRate;
        m_FullScreenDesc.RefreshRate.Denominator = 1;
        m_FullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
        m_FullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        m_FullScreenDesc.Windowed = !m_DeviceParams.startFullscreen;

        RefCountPtr<IDXGISwapChain1> pSwapChain1;
        hr = pDxgiFactory->CreateSwapChainForHwnd(m_GraphicsQueue, m_hWnd, &m_SwapChainDesc, &m_FullScreenDesc, nullptr, &pSwapChain1);
        HR_RETURN(hr);

        hr = pSwapChain1->QueryInterface(IID_PPV_ARGS(&m_SwapChain));
        HR_RETURN(hr);

        nvrhi::d3d12::DeviceDesc deviceDesc;
        deviceDesc.errorCB = &DefaultMessageCallback::GetInstance();
        deviceDesc.pDevice = m_Device12;
        deviceDesc.pGraphicsCommandQueue = m_GraphicsQueue;
        deviceDesc.pComputeCommandQueue = m_ComputeQueue;
        deviceDesc.pCopyCommandQueue = m_CopyQueue;

        m_NvrhiDevice = nvrhi::d3d12::createDevice(deviceDesc);

        if (m_DeviceParams.enableNvrhiValidationLayer)
        {
            m_NvrhiDevice = nvrhi::validation::createValidationLayer(m_NvrhiDevice);
        }

        if (!CreateRenderTargets())
        {
            return false;
        }

        hr = m_Device12->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_FrameFence));
        HR_RETURN(hr);

        for (UINT bufferIndex = 0; bufferIndex < m_SwapChainDesc.BufferCount; bufferIndex++)
        {
            m_FrameFenceEvents.push_back(CreateEvent(nullptr, false, true, NULL));
        }

        return true;
	}

    void DeviceManagerDX12::DestroyDeviceAndSwapChain()
    {
        m_RhiSwapChainBuffers.clear();
        m_RendererString.clear();

        ReleaseRenderTargets();

        m_NvrhiDevice = nullptr;

        for (auto fenceEvent : m_FrameFenceEvents)
        {
            WaitForSingleObject(fenceEvent, INFINITE);
            CloseHandle(fenceEvent);
        }

        m_FrameFenceEvents.clear();

        if (m_SwapChain)
        {
            m_SwapChain->SetFullscreenState(false, nullptr);
        }

        m_SwapChainBuffers.clear();

        m_FrameFence = nullptr;
        m_SwapChain = nullptr;
        m_GraphicsQueue = nullptr;
        m_ComputeQueue = nullptr;
        m_CopyQueue = nullptr;
        m_Device12 = nullptr;
        m_DxgiAdapter = nullptr;
    }

    void DeviceManagerDX12::Render()
    {
        D_LOG_CRITICAL("NOT IMPLEMENTED");
    }

    void DeviceManagerDX12::CreateRenderPass(const RenderPassDesc& renderPassDesc, RenderPass* renderPass)
    {
        D_LOG_CRITICAL("NOT IMPLEMENTED");
    }

    void DeviceManagerDX12::CreateCommandBuffer(CommandBuffer* commandBuffer) const
    {
        D_LOG_CRITICAL("NOT IMPLEMENTED");
    }

    nvrhi::GraphicsAPI DeviceManagerDX12::GetGraphicsAPI() const
    {
        return nvrhi::GraphicsAPI::D3D12;
    }

    uint32_t DeviceManagerDX12::GetBackBufferCount()
    {
        return m_SwapChainDesc.BufferCount;
    }

    void DeviceManagerDX12::ResizeSwapChain()
    {
        ReleaseRenderTargets();

        if (!m_NvrhiDevice)
            return;

        if (!m_SwapChain)
            return;

        const HRESULT hr = m_SwapChain->ResizeBuffers(m_DeviceParams.bufferCount,
            m_DeviceParams.backBufferWidth,
            m_DeviceParams.backBufferHeight,
            m_SwapChainDesc.Format,
            m_SwapChainDesc.Flags);

        if (FAILED(hr))
        {
            D_LOG_CRITICAL("ResizeBuffers failed");
        }

        bool ret = CreateRenderTargets();
        if (!ret)
        {
            D_LOG_CRITICAL("CreateRenderTarget failed");
        }
    }

    nvrhi::ITexture* DeviceManagerDX12::GetBackBuffer(uint32_t index)
    {
        if (index < m_RhiSwapChainBuffers.size())
            return m_RhiSwapChainBuffers[index];
        return nullptr;
    }

    RefCountPtr<IDXGIAdapter> DeviceManagerDX12::GetHardwareAdapter(const std::wstring& targetName)
    {
        RefCountPtr<IDXGIAdapter> targetAdapter;
        RefCountPtr<IDXGIFactory1> DXGIFactory;
        HRESULT hres = CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory));
        if (hres != S_OK)
        {
            D_LOG_CRITICAL("An error occurred in `CreateDXGIFactory`!");
            return targetAdapter;
        }

        unsigned int adapterNo = 0;
        while (SUCCEEDED(hres))
        {
            RefCountPtr<IDXGIAdapter> pAdapter;
            hres = DXGIFactory->EnumAdapters(adapterNo, &pAdapter);

            if (SUCCEEDED(hres))
            {
                DXGI_ADAPTER_DESC aDesc;
                pAdapter->GetDesc(&aDesc);

                if (targetName.length() == 0)
                {
                    targetAdapter = pAdapter;
                    break;
                }

                std::wstring aName = aDesc.Description;

                if (aName.find(targetName) != std::string::npos)
                {
                    targetAdapter = pAdapter;
                    break;
                }
            }

            adapterNo++;
        }

        return targetAdapter;
    }

    inline int ComputeIntersectionArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
    {
        return std::max(0, std::min(ax2, bx2) - std::max(ax1, bx1)) * std::max(0, std::min(ay2, by2) - std::max(ay1, by1));
    }

}
