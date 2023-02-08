#include "Derydoca/Rendering/DX12/DeviceManagerDX12.h"
#include "Derydoca/Logging/Log.h"
#include "Derydoca/Rendering/DX12/DXHelper.h"

#include <string>
#include <fmt/xchar.h>
#include <sstream>
#include <SDL2/SDL_syswm.h>

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

    DeviceManagerDX12::DeviceManagerDX12(const DeviceManagerSettings& deviceSettings, SDL_Window* sdlWindow)
        : DeviceManager(deviceSettings, sdlWindow)
    {
        CreateDeviceAndSwapChain();
    }

    bool DeviceManagerDX12::CreateRenderTargets()
    {
        m_SwapChainBuffers.resize(m_SwapChainDesc.BufferCount);
        m_RhiSwapChainBuffers.resize(m_SwapChainDesc.BufferCount);

        for (UINT n = 0; n < m_SwapChainDesc.BufferCount; n++)
        {
            const HRESULT hr = m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_SwapChainBuffers[n]));
            HR_RETURN(hr);

            nvrhi::TextureDesc textureDesc;
            textureDesc.width = m_DeviceSettings.width;
            textureDesc.height = m_DeviceSettings.height;
            textureDesc.sampleCount = m_DeviceSettings.sampleCount;
            textureDesc.sampleQuality = m_DeviceSettings.sampleQuality;
            textureDesc.format = m_DeviceSettings.imageFormat;
            textureDesc.debugName = "SwapChainBuffer";
            textureDesc.isRenderTarget = true;
            textureDesc.isUAV = false;
            textureDesc.initialState = nvrhi::ResourceStates::Present;
            textureDesc.keepInitialState = true;

            m_RhiSwapChainBuffers[n] = m_nvrhiDevice->createHandleForNativeTexture(nvrhi::ObjectTypes::D3D12_Resource, nvrhi::Object(m_SwapChainBuffers[n]), textureDesc);
        }

        return true;
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
        RECT rect = { 0, 0, LONG(m_DeviceSettings.width), LONG(m_DeviceSettings.height) };

        RefCountPtr<IDXGIAdapter> targetAdapter;
        targetAdapter = GetHardwareAdapter(m_DeviceSettings.adapterNameSubstring);

        if (!targetAdapter)
        {
            std::wstring adapterNameStr(m_DeviceSettings.adapterNameSubstring.begin(), m_DeviceSettings.adapterNameSubstring.end());

            D_LOG_CRITICAL("Unable to find adapter with the name filter of '{0}'.", "adapterNameStr");
            return false;
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
            SDL_SetWindowPosition(window, rect.left, rect.top);
        }

        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(window, &wmInfo);
        m_hWnd = wmInfo.info.win.window;

        HRESULT hr = E_FAIL;

        RECT clientRect;
        GetClientRect(m_hWnd, &clientRect);
        UINT width = clientRect.right - clientRect.left;
        UINT height = clientRect.bottom - clientRect.top;

        ZeroMemory(&m_SwapChainDesc, sizeof(m_SwapChainDesc));
        m_SwapChainDesc.Width = width;
        m_SwapChainDesc.Height = height;
        m_SwapChainDesc.SampleDesc.Count = m_DeviceSettings.sampleCount;
        m_SwapChainDesc.SampleDesc.Quality = 0;
        m_SwapChainDesc.BufferUsage = m_DeviceSettings.swapChainUsage;
        m_SwapChainDesc.BufferCount = m_DeviceSettings.bufferCount;
        m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        m_SwapChainDesc.Flags = m_DeviceSettings.allowModeSwitch ? DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH : 0;

        // Special processing for sRGB swap chain formats.
        // DXGI will not create a swap chain with an sRGB format, but its contents will be interpreted as sRGB.
        // So we need to use a non-sRGB format here, but store the true sRGB format for later framebuffer creation.
        switch (m_DeviceSettings.imageFormat)  // NOLINT(clang-diagnostic-switch-enum)
        {
        case nvrhi::Format::SRGBA8_UNORM:
            m_SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        case nvrhi::Format::SBGRA8_UNORM:
            m_SwapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            break;
        default:
            m_SwapChainDesc.Format = nvrhi::d3d12::convertFormat(m_DeviceSettings.imageFormat);
            break;
        }

        if (m_DeviceSettings.enableDebugRuntime)
        {
            RefCountPtr<ID3D12Debug> pDebug;
            hr = D3D12GetDebugInterface(IID_PPV_ARGS(&pDebug));
            HR_RETURN(hr);

            pDebug->EnableDebugLayer();
        }

        RefCountPtr<IDXGIFactory2> pDxgiFactory;
        UINT dxgiFactoryFlags = m_DeviceSettings.enableDebugRuntime ? DXGI_CREATE_FACTORY_DEBUG : 0;
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
            m_DeviceSettings.featureLevel,
            IID_PPV_ARGS(&m_device));
        HR_RETURN(hr);

        if (m_DeviceSettings.enableDebugRuntime)
        {
            RefCountPtr<ID3D12InfoQueue> pInfoQueue;
            m_device->QueryInterface(&pInfoQueue);

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
        hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_GraphicsQueue));
        HR_RETURN(hr);
        m_GraphicsQueue->SetName(L"Graphics Queue");

        if (m_DeviceSettings.enableComputeQueue)
        {
            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
            hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_ComputeQueue));
            HR_RETURN(hr);
            m_ComputeQueue->SetName(L"Compute Queue");
        }

        if (m_DeviceSettings.enableCopyQueue)
        {
            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
            hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CopyQueue));
            HR_RETURN(hr);
            m_CopyQueue->SetName(L"Copy Queue");
        }

        m_FullScreenDesc = {};
        m_FullScreenDesc.RefreshRate.Numerator = m_DeviceSettings.refreshRate;
        m_FullScreenDesc.RefreshRate.Denominator = 1;
        m_FullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
        m_FullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        m_FullScreenDesc.Windowed = !m_DeviceSettings.startFullscreen;

        RefCountPtr<IDXGISwapChain1> pSwapChain1;
        hr = pDxgiFactory->CreateSwapChainForHwnd(m_GraphicsQueue, m_hWnd, &m_SwapChainDesc, &m_FullScreenDesc, nullptr, &pSwapChain1);
        HR_RETURN(hr);

        hr = pSwapChain1->QueryInterface(IID_PPV_ARGS(&m_swapChain));
        HR_RETURN(hr);

        nvrhi::d3d12::DeviceDesc deviceDesc;
        deviceDesc.errorCB = &DefaultMessageCallback::GetInstance();
        deviceDesc.pDevice = m_device;
        deviceDesc.pGraphicsCommandQueue = m_GraphicsQueue;
        deviceDesc.pComputeCommandQueue = m_ComputeQueue;
        deviceDesc.pCopyCommandQueue = m_CopyQueue;

        m_nvrhiDevice = nvrhi::d3d12::createDevice(deviceDesc);

        if (m_DeviceSettings.enableNvrhiValidationLayer)
        {
            m_nvrhiDevice = nvrhi::validation::createValidationLayer(m_nvrhiDevice);
        }

        if (!CreateRenderTargets())
        {
            return false;
        }

        hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_FrameFence));
        HR_RETURN(hr);

        for (UINT bufferIndex = 0; bufferIndex < m_SwapChainDesc.BufferCount; bufferIndex++)
        {
            m_FrameFenceEvents.push_back(CreateEvent(nullptr, false, true, NULL));
        }

        return true;
	}
    
    void DeviceManagerDX12::Render()
    {
        ////vkWaitForFences
        //if (m_fences[m_frameIndex]->GetCompletedValue() < m_fenceValues[m_frameIndex])
        //{
        //    ThrowIfFailed(m_fences[m_frameIndex]->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvents[m_frameIndex]));
        //    WaitForSingleObjectEx(m_fenceEvents[m_frameIndex], INFINITE, FALSE);
        //}

        ////vkAcquireNextImageKHR
        //m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

        //// Swapchain recreation if resized
        //if (framebufferResized)
        //{
        //    D_LOG_WARN("Window was resized but framebuffer resizing has yet to be implemented in the DX12 renderer!");
        //    framebufferResized = false;
        //}

        ////vkResetFences
        //m_fenceValues[m_frameIndex]++;

        ////vkAllocateCommandBuffers;vkBeginCommandBuffer
        //ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[m_frameIndex].Get(), nullptr, IID_PPV_ARGS(&m_commandList)));
        //m_commandList->SetName(L"Main");

        ////vkCmdSetViewport
        //{
        //    D3D12_VIEWPORT viewport = {};
        //    viewport.Height = 1080;
        //    viewport.Width = 1920;
        //    viewport.MinDepth = D3D12_MIN_DEPTH;
        //    viewport.MaxDepth = D3D12_MAX_DEPTH;

        //    m_commandList->RSSetViewports(1, &viewport);
        //}

        ////vkCmdSetScissor
        //{
        //    D3D12_RECT scissorRect = {};
        //    scissorRect.right = 1080;
        //    scissorRect.bottom = 1920;

        //    m_commandList->RSSetScissorRects(1, &scissorRect);
        //}

        ////vkCmdBeginRenderPass
        //{
        //    static float t = 0.0f;
        //    t += 0.05f;
        //    float g = (sin(t) + 1.0) * 0.5f;
        //    const float clearColor[] = { 0.2, g, 0.4f, 1.0f };

        //    int numTargets = 1;
        //    auto clearValues = std::vector<ClearValue>(numTargets);
        //    clearValues[0].Color.float32[0] = clearColor[0];
        //    clearValues[0].Color.float32[1] = clearColor[1];
        //    clearValues[0].Color.float32[2] = clearColor[2];
        //    clearValues[0].Color.float32[3] = clearColor[3];

        //    auto targets = std::vector<RenderTarget*>(numTargets);
        //    targets[0] = &m_renderTargetHandles[m_frameIndex];

        //    RenderPassBeginInfo beginInfo{};
        //    beginInfo.Area.offset = { 0,0 };
        //    beginInfo.Area.extent = { 1080, 1920 };
        //    beginInfo.ClearValueCount = clearValues.size();
        //    beginInfo.ClearValues = clearValues.data();
        //    beginInfo.Targets = targets.data();

        //    // Command buffer begin render pass: subpass, m_renderPass

        //    auto subpass = m_renderPass.Subpasses[0];

        //    m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_swapChainBuffers[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

        //    for (size_t targetIndex = 0; targetIndex < beginInfo.ClearValueCount; targetIndex++)
        //    {
        //        RenderTarget* rt = beginInfo.Targets[0];
        //        subpass.RenderTargets[targetIndex].cpuDescriptor = *static_cast<D3D12_CPU_DESCRIPTOR_HANDLE*>(rt);
        //        CD3DX12_CLEAR_VALUE clearValue{ subpass.RenderTargets[targetIndex].BeginningAccess.Clear.ClearValue.Format, beginInfo.ClearValues[0].Color.float32 };
        //        subpass.RenderTargets[0].BeginningAccess.Clear.ClearValue = clearValue;
        //    }

        //    if (subpass.DepthStencil.has_value())
        //    {
        //        auto& depthStencilClear = subpass.DepthStencil.value();

        //        depthStencilClear.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Depth = beginInfo.DepthStencil->DepthStencil.Depth;
        //        depthStencilClear.StencilBeginningAccess.Clear.ClearValue.DepthStencil.Depth = beginInfo.DepthStencil->DepthStencil.Stencil;
        //    }

        //    m_commandList->BeginRenderPass(
        //        subpass.RenderTargets.size(),
        //        subpass.RenderTargets.data(),
        //        subpass.DepthStencil.has_value() ? &subpass.DepthStencil.value() : nullptr,
        //        subpass.Flags
        //    );
        //}

        ////vkCmdEndRenderPass
        //{
        //    m_commandList->EndRenderPass();
        //    m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_swapChainBuffers[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
        //}


        ////vkEndCommandBuffer
        //ThrowIfFailed(m_commandList->Close());

        ////vkQueueSubmit
        //ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
        //m_GraphicsCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        ////vkQueuePresentKHR
        //ThrowIfFailed(m_swapChain->Present(1, 0));

        //const UINT currentFenceValue = m_fenceValues[m_frameIndex];
        //ThrowIfFailed(m_GraphicsCommandQueue->Signal(m_fences[m_frameIndex].Get(), currentFenceValue));
    }

    void DeviceManagerDX12::CreateRenderPass(const RenderPassDesc& renderPassDesc, RenderPass* renderPass)
    {
        //RenderPassDX12* renderPassDX12 = static_cast<RenderPassDX12*>(renderPass);
        //if (renderPassDX12 == nullptr)
        //{
        //    D_LOG_TRACE("Creating render pass...");
        //    renderPassDX12 = new RenderPassDX12();
        //}

        //renderPassDX12->Subpasses.resize(renderPassDesc.AttachmentCount);
        //for (size_t subpassIndex = 0; subpassIndex < renderPassDesc.SubpassCount; subpassIndex++)
        //{
        //    auto& subpass = renderPassDesc.Subpasses[subpassIndex];
        //    auto& dxSubpass = renderPassDX12->Subpasses[subpassIndex];
        //    dxSubpass.RenderTargets.resize(subpass.ColorAttachmentCount);
        //    for (size_t attachmentIndex = 0; attachmentIndex < subpass.ColorAttachmentCount; attachmentIndex++)
        //    {
        //        auto& subpassColorAttachment = subpass.ColorAttachments[attachmentIndex];
        //        auto& colorAttachment = renderPassDesc.RenderTargets[subpassColorAttachment.Attachment];
        //        auto& dxColorAttachment = dxSubpass.RenderTargets[attachmentIndex];

        //        // Properties to be set via render pass begin:
        //        //    dxColorAttachment.BeginningAccess.Clear.ClearValue.Color
        //        //    dxColorAttachment.cpuDescriptor

        //        dxColorAttachment.BeginningAccess.Clear.ClearValue.Format = Translate(colorAttachment.Format);
        //        dxColorAttachment.BeginningAccess.Type = Translate(colorAttachment.BeginningAccess);
        //        // TODO: Add support for multisample resolve textures
        //        dxColorAttachment.EndingAccess.Resolve = {};
        //        dxColorAttachment.EndingAccess.Type = Translate(colorAttachment.EndingAccess);
        //    }

        //    if (renderPassDesc.DepthStencil != nullptr)
        //    {
        //        D3D12_RENDER_PASS_DEPTH_STENCIL_DESC depthStencilDesc;

        //        // Properties to be set via render pass begin:
        //        //    depthStencilDesc.cpuDescriptor
        //        //    depthStencilDesc.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Depth
        //        //    depthStencilDesc.StencilBeginningAccess.Clear.ClearValue.DepthStencil.Stencil

        //        depthStencilDesc.DepthBeginningAccess.Clear.ClearValue.Format = Translate(renderPassDesc.DepthStencil->Format);
        //        depthStencilDesc.DepthBeginningAccess.Type = Translate(renderPassDesc.DepthStencil->DepthBeginningAccess);
        //        depthStencilDesc.DepthEndingAccess.Resolve = {};
        //        depthStencilDesc.DepthEndingAccess.Type = Translate(renderPassDesc.DepthStencil->DepthEndingAccess);
        //        depthStencilDesc.StencilBeginningAccess.Type = Translate(renderPassDesc.DepthStencil->StencilBeginningAccess);
        //        depthStencilDesc.StencilEndingAccess.Resolve = {};
        //        depthStencilDesc.StencilEndingAccess.Type = Translate(renderPassDesc.DepthStencil->StencilEndingAccess);
        //    }
        //    else
        //    {
        //        dxSubpass.DepthStencil.reset();
        //    }

        //    // TODO: Add support for DX render pass flags
        //    dxSubpass.Flags = D3D12_RENDER_PASS_FLAG_NONE;
        //}
    }

    void DeviceManagerDX12::CreateCommandBuffer(CommandBuffer* commandBuffer) const
    {
        D_LOG_CRITICAL("NOT IMPLEMENTED");
    }

    //void DeviceManagerDX12::CheckTearingSupport()
    //{
    //    ComPtr<IDXGIFactory6> factory;
    //    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    //    BOOL allowTearing = FALSE;
    //    if (SUCCEEDED(hr))
    //    {
    //        hr = factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
    //    }

    //    m_tearingSupport = SUCCEEDED(hr) && allowTearing;
    //}

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

    //void DeviceManagerDX12::PrintDisplayColorSpaceInfo(SDL_Window* window)
    //{
    //    if (m_dxgiFactory->IsCurrent() == false)
    //    {
    //        ThrowIfFailed(
    //            CreateDXGIFactory2(0, IID_PPV_ARGS(&m_dxgiFactory))
    //        );
    //    }

    //    ComPtr<IDXGIAdapter1> dxgiAdapter;
    //    ThrowIfFailed(m_dxgiFactory->EnumAdapters1(0, &dxgiAdapter));

    //    UINT i = 0;
    //    ComPtr<IDXGIOutput> currentOutput;
    //    ComPtr<IDXGIOutput> bestOutput;
    //    float bestIntersectArea = -1;

    //    // Get the window bounds
    //    int ax1 = 0;
    //    int ay1 = 0;
    //    int ax2 = 0;
    //    int ay2 = 0;
    //    SDL_GetWindowPosition(window, &ax1, &ay1);
    //    SDL_GetWindowSize(window, &ax2, &ay2);
    //    ax2 += ax1;
    //    ay2 += ay1;

    //    while (dxgiAdapter->EnumOutputs(i, &currentOutput) != DXGI_ERROR_NOT_FOUND)
    //    {
    //        DXGI_OUTPUT_DESC desc;
    //        ThrowIfFailed(currentOutput->GetDesc(&desc));
    //        RECT r = desc.DesktopCoordinates;
    //        int bx1 = r.left;
    //        int by1 = r.top;
    //        int bx2 = r.right;
    //        int by2 = r.bottom;

    //        int intersectArea = ComputeIntersectionArea(ax1, ay1, ax2, ay2, bx1, by1, bx2, by2);
    //        if (intersectArea > bestIntersectArea)
    //        {
    //            bestOutput = currentOutput;
    //            bestIntersectArea = static_cast<float>(intersectArea);
    //        }

    //        i++;
    //    }

    //    ComPtr<IDXGIOutput6> output6;
    //    ThrowIfFailed(bestOutput.As(&output6));

    //    DXGI_OUTPUT_DESC1 desc1;
    //    ThrowIfFailed(output6->GetDesc1(&desc1));

    //    std::string rotation = "UNDEFINED";
    //    if (desc1.Rotation >= 0) {
    //        rotation = fmt::format("{} degrees", (desc1.Rotation - 1) * 90);
    //    }

    //    D_LOG_INFO(
    //        "\nActive Display Info:\n"
    //        "  Device Name: {}\n"
    //        "  Bits Per Color: {}\n"
    //        "  Max Luminance: {}\n"
    //        "  Min Luminance: {}\n"
    //        "  Max Full Frame Luminance: {}\n"
    //        "  Rotation: {}\n"
    //        "  White Point: [{}, {}]\n"
    //        "  R Primary: [{}, {}]\n"
    //        "  G Primary: [{}, {}]\n"
    //        "  B Primary: [{}, {}]",
    //        CharArrToStr(desc1.DeviceName, 128),
    //        desc1.BitsPerColor,
    //        desc1.MaxLuminance,
    //        desc1.MinLuminance,
    //        desc1.MaxFullFrameLuminance,
    //        rotation,
    //        desc1.WhitePoint[0], desc1.WhitePoint[1],
    //        desc1.RedPrimary[0], desc1.RedPrimary[1],
    //        desc1.GreenPrimary[0], desc1.GreenPrimary[1],
    //        desc1.BluePrimary[0], desc1.BluePrimary[1]
    //    );
    //}
}
