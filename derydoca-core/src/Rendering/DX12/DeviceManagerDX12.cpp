#include "Derydoca/Rendering/DX12/DeviceManagerDX12.h"
#include "Derydoca/Logging/Log.h"

#include <sstream>

#define HR_RETURN(hr) if(FAILED(hr)) return false

namespace DerydocaEngine::Rendering
{
    // Find an adapter whose name contains the given string.
    static nvrhi::RefCountPtr<IDXGIAdapter> FindAdapter(const std::wstring& targetName)
    {
        nvrhi::RefCountPtr<IDXGIAdapter> targetAdapter;
        nvrhi::RefCountPtr<IDXGIFactory1> DXGIFactory;
        HRESULT hres = CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory));
        if (hres != S_OK)
        {
            D_LOG_CRITICAL("Unable to find graphics adapter!");
            return targetAdapter;
        }

        unsigned int adapterNo = 0;
        while (SUCCEEDED(hres))
        {
            nvrhi::RefCountPtr<IDXGIAdapter> pAdapter;
            hres = DXGIFactory->EnumAdapters(adapterNo, &pAdapter);

            if (SUCCEEDED(hres))
            {
                DXGI_ADAPTER_DESC aDesc;
                pAdapter->GetDesc(&aDesc);

                // If no name is specified, return the first adapater.  This is the same behaviour as the
                // default specified for D3D11CreateDevice when no adapter is specified.
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

    // Adjust window rect so that it is centred on the given adapter.  Clamps to fit if it's too big.
    static bool MoveWindowOntoAdapter(IDXGIAdapter* targetAdapter, RECT& rect)
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
		UINT windowStyle = (WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		RECT rect = { 0, 0, LONG(m_WindowParams.BackbufferWidth), LONG(m_WindowParams.BackbufferHeight) };
		AdjustWindowRect(&rect, windowStyle, FALSE);

		nvrhi::RefCountPtr<IDXGIAdapter> targetAdapter;

		if (m_WindowParams.Adapter)
		{
			targetAdapter = m_WindowParams.Adapter;
		}
		else
		{
			targetAdapter = FindAdapter(m_WindowParams.AdapterNameSubstring);

			if (!targetAdapter)
			{
				std::wstring adapterNameStr(m_WindowParams.AdapterNameSubstring.begin(), m_WindowParams.AdapterNameSubstring.end());

				//D_LOG_ERROR("Unable to find an adapter matching {}", adapterNameStr.c_str());
				return false;
			}
		}

        {
            DXGI_ADAPTER_DESC adapterDesc;
            targetAdapter->GetDesc(&adapterDesc);

            std::wstring adapterName = adapterDesc.Description;

            // A stupid but non-deprecated and portable way of converting a wstring to a string
            std::stringstream ss;
            std::wstringstream wss;
            for (auto c : adapterName)
                ss << wss.narrow(c, '?');
            m_RendererString = ss.str();
        }

        if (MoveWindowOntoAdapter(targetAdapter, rect))
        {
            glfwSetWindowPos(m_Window, rect.left, rect.top);
        }

        m_hWnd = glfwGetWin32Window(m_Window);

        RECT clientRect;
        GetClientRect(m_hWnd, &clientRect);
        UINT width = clientRect.right - clientRect.left;
        UINT height = clientRect.bottom - clientRect.top;

        ZeroMemory(&m_SwapChainDesc, sizeof(m_SwapChainDesc));
        m_SwapChainDesc.Width = width;
        m_SwapChainDesc.Height = height;
        m_SwapChainDesc.SampleDesc.Count = m_WindowParams.SwapChainSampleCount;
        m_SwapChainDesc.SampleDesc.Quality = 0;
        m_SwapChainDesc.BufferUsage = m_WindowParams.SwapChainUsage; //?
        m_SwapChainDesc.BufferCount = m_WindowParams.SwapChainBufferCount;
        m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        m_SwapChainDesc.Flags = m_WindowParams.AllowModeSwitch ? DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH : 0; //?

        switch (m_WindowParams.SwapChainFormat)
        {
        case nvrhi::Format::SRGBA8_UNORM:
            m_SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        case nvrhi::Format::SBGRA8_UNORM:
            m_SwapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            break;
        default:
            m_SwapChainDesc.Format = nvrhi::d3d12::convertFormat(m_WindowParams.SwapChainFormat);
            break;
        }

        HRESULT hr = E_FAIL;
        if (m_WindowParams.EnableDebugRuntime)
        {
            nvrhi::RefCountPtr<ID3D12Debug> d3dDebug;
            hr = D3D12GetDebugInterface(IID_PPV_ARGS(&d3dDebug));
            HR_RETURN(hr);

            d3dDebug->EnableDebugLayer();
        }

        nvrhi::RefCountPtr<IDXGIFactory2> dxgiFactory;
        UINT dxgiFactoryFlags = m_WindowParams.EnableDebugRuntime ? DXGI_CREATE_FACTORY_DEBUG : 0;
        hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
        HR_RETURN(hr);

        nvrhi::RefCountPtr<IDXGIFactory5> dxgiFactory5;
        if (SUCCEEDED(dxgiFactory->QueryInterface(IID_PPV_ARGS(&dxgiFactory5))))
        {
            BOOL supported = 0;
            if (SUCCEEDED(dxgiFactory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &supported, sizeof(supported))))
            {
                m_TearingSupported = (supported != 0);
            }
        }

        if (m_TearingSupported)
        {
            m_SwapChainDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
        }

        hr = D3D12CreateDevice(targetAdapter, m_WindowParams.FeatureLevel, IID_PPV_ARGS(&m_Device12));
        HR_RETURN(hr);

        if (m_WindowParams.EnableDebugRuntime)
        {
            nvrhi::RefCountPtr<ID3D12InfoQueue> infoQueue;
            m_Device12->QueryInterface(&infoQueue);

            if (infoQueue)
            {
#ifdef _DEBUG
                infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
                infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
#endif

                D3D12_MESSAGE_ID disableMessageIDs[] = {
                    D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
                    D3D12_MESSAGE_ID_COMMAND_LIST_STATIC_DESCRIPTOR_RESOURCE_DIMENSION_MISMATCH,
                };

                D3D12_INFO_QUEUE_FILTER filter = {};
                filter.DenyList.pIDList = disableMessageIDs;
                filter.DenyList.NumIDs = sizeof(disableMessageIDs) / sizeof(disableMessageIDs[0]);
                infoQueue->AddStorageFilterEntries(&filter);
            }
        }

        m_DxgiAdapter = targetAdapter;

        D3D12_COMMAND_QUEUE_DESC queueDesc;
        ZeroMemory(&queueDesc, sizeof(queueDesc));
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queueDesc.NodeMask = 1; //?
        hr = m_Device12->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_GraphicsQueue));
        HR_RETURN(hr);
        m_GraphicsQueue->SetName(L"Graphics Queue");

        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
        hr = m_Device12->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_ComputeQueue));
        HR_RETURN(hr);
        m_ComputeQueue->SetName(L"Compute Queue");

        if (m_WindowParams.EnableCopyQueue)
        {
            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
            hr = m_Device12->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CopyQueue));
            HR_RETURN(hr);
            m_CopyQueue->SetName(L"Copy Queue");
        }

        m_FullScreenDesc = {};
        m_FullScreenDesc.RefreshRate.Numerator = m_WindowParams.RefreshRate;
        m_FullScreenDesc.RefreshRate.Denominator = 1;
        m_FullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
        m_FullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        m_FullScreenDesc.Windowed = !m_WindowParams.StartFullscreen;

        nvrhi::RefCountPtr<IDXGISwapChain1> swapChain1;
        hr = dxgiFactory->CreateSwapChainForHwnd(m_GraphicsQueue, m_hWnd, &m_SwapChainDesc, &m_FullScreenDesc, nullptr, &swapChain1);
        HR_RETURN(hr);

        hr = swapChain1->QueryInterface(IID_PPV_ARGS(&m_SwapChain));
        HR_RETURN(hr);

        nvrhi::d3d12::DeviceDesc deviceDesc;
        deviceDesc.errorCB = nullptr;
        deviceDesc.pDevice = m_Device12;
        deviceDesc.pGraphicsCommandQueue = m_GraphicsQueue;
        deviceDesc.pComputeCommandQueue = m_ComputeQueue;
        deviceDesc.pCopyCommandQueue = m_CopyQueue;

        m_NvrhiDevice = nvrhi::d3d12::createDevice(deviceDesc);

        if (m_WindowParams.EnableNvrhiValidationLayer)
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

    bool DeviceManagerDX12::CreateRenderTargets()
    {
        m_SwapChainBuffers.resize(m_SwapChainDesc.BufferCount);
        m_RhiSwapChainBuffers.resize(m_SwapChainDesc.BufferCount);

        for (UINT n = 0; n < m_SwapChainDesc.BufferCount; n++)
        {
            const HRESULT hr = m_SwapChain->GetBuffer(n, IID_PPV_ARGS(&m_SwapChainBuffers[n]));
            HR_RETURN(hr);

            nvrhi::TextureDesc textureDesc;
            textureDesc.width = m_WindowParams.BackbufferWidth;
            textureDesc.height = m_WindowParams.BackbufferHeight;
            textureDesc.sampleCount = m_WindowParams.SwapChainSampleCount;
            textureDesc.sampleQuality = m_WindowParams.SwapChainSampleQuality;
            textureDesc.format = m_WindowParams.SwapChainFormat;
            textureDesc.debugName = "SwapChainBuffer";
            textureDesc.isRenderTarget = true;
            textureDesc.isUAV = false;
            textureDesc.initialState = nvrhi::ResourceStates::Present;
            textureDesc.keepInitialState = true;

            m_RhiSwapChainBuffers[n] = m_NvrhiDevice->createHandleForNativeTexture(nvrhi::ObjectTypes::D3D12_Resource, nvrhi::Object(m_SwapChainBuffers[n]), textureDesc);
        }

        return true;
    }

    void DeviceManagerDX12::Present()
    {
        if (!m_WindowVisible)
        {
            return;
        }

        auto bufferIndex = m_SwapChain->GetCurrentBackBufferIndex();

        UINT presentFlags = 0;
        if (!m_WindowParams.VSyncEnabled && m_FullScreenDesc.Windowed && m_TearingSupported)
        {
            presentFlags |= DXGI_PRESENT_ALLOW_TEARING;
        }

        m_SwapChain->Present(m_WindowParams.VSyncEnabled ? 1 : 0, presentFlags);

        m_FrameFence->SetEventOnCompletion(m_FrameCount, m_FrameFenceEvents[bufferIndex]);
        m_GraphicsQueue->Signal(m_FrameFence, m_FrameCount);
        m_FrameCount++;
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
        m_CopyQueue = nullptr;
        m_Device12 = nullptr;
        m_DxgiAdapter = nullptr;
	}

    void DeviceManagerDX12::ReleaseRenderTargets()
    {
        m_NvrhiDevice->waitForIdle();
        m_NvrhiDevice->runGarbageCollection();
        for (auto fenceEvent : m_FrameFenceEvents)
        {
            SetEvent(fenceEvent);
        }
        m_RhiSwapChainBuffers.clear();
        m_SwapChainBuffers.clear();
    }
}
