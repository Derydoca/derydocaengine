#include "Derydoca/Rendering/DX12/DeviceManagerDX12.h"
#include "Derydoca/Logging/Log.h"
#include "Derydoca/Rendering/DX12/DXHelper.h"

#include <string>

#define D3D_ACTIVE_FEATURE_LEVEL D3D_FEATURE_LEVEL_12_1

namespace Derydoca::Rendering
{
	void DeviceManagerDX12::Initialize(const DeviceManagerSettings& settings)
	{
		UINT dxgiFactoryFlags = 0;

#ifdef _DEBUG
        {
            ComPtr<ID3D12Debug> debugController;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
            {
                debugController->EnableDebugLayer();

                dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
            }
        }
#endif

		ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_dxgiFactory)));

        ComPtr<IDXGIAdapter1> hardwareAdapter;
        GetHardwareAdapter(m_dxgiFactory.Get(), &hardwareAdapter);

        ThrowIfFailed(D3D12CreateDevice(
            hardwareAdapter.Get(),
            D3D_ACTIVE_FEATURE_LEVEL,
            IID_PPV_ARGS(&m_device)
        ));

        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_renderingCommandQueue)));
        NAME_D3D_OBJECT(m_renderingCommandQueue);

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount = FrameCount;
        swapChainDesc.Width = settings.width;
        swapChainDesc.Height = settings.height;
        swapChainDesc.Format = Translate(settings.imageFormat);
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.SampleDesc.Count = 1;

        // It is recommended to always use the tearing flag when it is available.
        swapChainDesc.Flags = m_tearingSupport ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

        HWND osWindow = NULL;
        {
            SDL_SysWMinfo info = {};
            SDL_VERSION(&info.version);
            if (SDL_GetWindowWMInfo(window, &info))
            {
                if (info.subsystem != SDL_SYSWM_WINDOWS)
                {
                    D_LOG_CRITICAL("Incompatible SDL window subsystem! {0:x}", info.subsystem);
                    exit(-1);
                }
                osWindow = info.info.win.window;
            }
            if (osWindow == NULL)
            {
                D_LOG_CRITICAL("Unable to aquire the handle of the window!");
                exit(-1);
            }
        }

        ComPtr<IDXGISwapChain1> swapChain;
        ThrowIfFailed(m_dxgiFactory->CreateSwapChainForHwnd(
            m_renderingCommandQueue.Get(),
            osWindow,
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain
        ));

        if (m_tearingSupport)
        {
            m_dxgiFactory->MakeWindowAssociation(osWindow, DXGI_MWA_NO_ALT_ENTER);
        }

        ThrowIfFailed(swapChain.As(&m_swapChain));

        PrintDisplayColorSpaceInfo(window);

        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

        {
            D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
            rtvHeapDesc.NumDescriptors = FrameCount;
            rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

            m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        }

        {
            for (UINT n = 0; n < FrameCount; n++)
            {
                m_renderTargetHandles[n].InitOffsetted(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), n, m_rtvDescriptorSize);
                ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
                m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, m_renderTargetHandles[n]);

                ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[n])));
            }
        }


        SubpassDependency dependency{};
        dependency.srcSubpass = SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = PipelineStageFlags::ColorAttachmentOutput;
        dependency.srcAccessMask = AccessFlags::None;
        dependency.dstStageMask = PipelineStageFlags::ColorAttachmentOutput;
        dependency.dstAccessMask = AccessFlags::ColorAttachmentWrite;

        RenderPassRenderTargetDesc renderTargetDesc{};
        renderTargetDesc.Format = settings.imageFormat;
        renderTargetDesc.Samples = ImageSampleCount::_1;
        renderTargetDesc.BeginningAccess = RenderPassBeginningAccess::Clear;
        renderTargetDesc.EndingAccess = RenderPassEndingAccess::Preserve;
        renderTargetDesc.InitialLayout = ImageLayout::Undefined;
        renderTargetDesc.FinalLayout = ImageLayout::PresentSrc;

        AttachmentReference colorAttachmentRef{};
        colorAttachmentRef.Attachment = 0;
        colorAttachmentRef.Layout = ImageLayout::ColorAttachmentOptimal;

        SubpassDesc subpass{};
        subpass.BindPoint = PipelineBindPoint::Graphics;
        subpass.ColorAttachmentCount = 1;
        subpass.ColorAttachments = &colorAttachmentRef;

        RenderPassDesc renderPassDesc{};
        renderPassDesc.DependencyCount = 1;
        renderPassDesc.Dependencies = &dependency;
        renderPassDesc.AttachmentCount = 1;
        renderPassDesc.RenderTargets = &renderTargetDesc;
        renderPassDesc.SubpassCount = 1;
        renderPassDesc.Subpasses = &subpass;

        CreateRenderPass(renderPassDesc, static_cast<RenderPass*>(&m_renderPass));

        {
            for (UINT n = 0; n < FrameCount; n++)
            {
                ThrowIfFailed(m_device->CreateFence(m_fenceValues[m_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fences[n])));
                m_fenceValues[n]++;
                ThrowIfFailed(m_renderingCommandQueue->Signal(m_fences[m_frameIndex].Get(), m_fenceValues[m_frameIndex]));

                m_fenceEvents[n] = CreateEvent(nullptr, FALSE, TRUE, nullptr);
                if (m_fenceEvents[n] == nullptr)
                {
                    ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
                }
            }
        }

        for (UINT n = 0; n < FrameCount; n++)
        {
            // Wait for fence
            ThrowIfFailed(m_fences[m_frameIndex]->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvents[n]));
            WaitForSingleObjectEx(m_fenceEvents[n], INFINITE, FALSE);
        }
	}
    
    void DeviceManagerDX12::Render()
    {
        //vkWaitForFences
        if (m_fences[m_frameIndex]->GetCompletedValue() < m_fenceValues[m_frameIndex])
        {
            ThrowIfFailed(m_fences[m_frameIndex]->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvents[m_frameIndex]));
            WaitForSingleObjectEx(m_fenceEvents[m_frameIndex], INFINITE, FALSE);
        }

        //vkAcquireNextImageKHR
        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

        // Swapchain recreation if resized
        if (framebufferResized)
        {
            D_LOG_WARN("Window was resized but framebuffer resizing has yet to be implemented in the DX12 renderer!");
            framebufferResized = false;
        }

        //vkResetFences
        m_fenceValues[m_frameIndex]++;

        //vkAllocateCommandBuffers;vkBeginCommandBuffer
        ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[m_frameIndex].Get(), nullptr, IID_PPV_ARGS(&m_commandList)));

        //vkCmdSetViewport
        {
            D3D12_VIEWPORT viewport = {};
            viewport.Height = 1080;
            viewport.Width = 1920;
            viewport.MinDepth = D3D12_MIN_DEPTH;
            viewport.MaxDepth = D3D12_MAX_DEPTH;

            m_commandList->RSSetViewports(1, &viewport);
        }

        //vkCmdSetScissor
        {
            D3D12_RECT scissorRect = {};
            scissorRect.right = 1080;
            scissorRect.bottom = 1920;

            m_commandList->RSSetScissorRects(1, &scissorRect);
        }

        //vkCmdBeginRenderPass
        {
            static float t = 0.0f;
            t += 0.05f;
            float g = (sin(t) + 1.0) * 0.5f;
            const float clearColor[] = { 0.2, g, 0.4f, 1.0f };

            auto subpass = m_renderPass.Subpasses[0];

            subpass.RenderTargets[0].cpuDescriptor = m_renderTargetHandles[m_frameIndex];
            CD3DX12_CLEAR_VALUE clearValue{ subpass.RenderTargets[0].BeginningAccess.Clear.ClearValue.Format, clearColor };
            subpass.RenderTargets[0].BeginningAccess.Clear.ClearValue = clearValue;

            m_commandList->BeginRenderPass(
                subpass.RenderTargets.size(),
                subpass.RenderTargets.data(),
                subpass.DepthStencil.has_value() ? &subpass.DepthStencil.value() : nullptr,
                subpass.Flags
            );
        }

        //vkCmdEndRenderPass
        {
            m_commandList->EndRenderPass();
        }

        ThrowIfFailed(m_commandList->Close());

        //vkEndCommandBuffer
        m_commandList->Close();

        //vkQueueSubmit
        ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
        m_renderingCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        //vkQueuePresentKHR
        ThrowIfFailed(m_swapChain->Present(1, 0));

        const UINT currentFenceValue = m_fenceValues[m_frameIndex];
        ThrowIfFailed(m_renderingCommandQueue->Signal(m_fences[m_frameIndex].Get(), currentFenceValue));
    }

    void DeviceManagerDX12::Cleanup()
    {
    }

    void DeviceManagerDX12::CreateRenderPass(const RenderPassDesc& renderPassDesc, RenderPass* renderPass)
    {
        RenderPassDX12* renderPassDX12 = static_cast<RenderPassDX12*>(renderPass);
        if (renderPassDX12 == nullptr)
        {
            D_LOG_TRACE("Creating render pass...");
            renderPassDX12 = new RenderPassDX12();
        }

        renderPassDX12->Subpasses.resize(renderPassDesc.AttachmentCount);
        for (size_t subpassIndex = 0; subpassIndex < renderPassDesc.SubpassCount; subpassIndex++)
        {
            auto& subpass = renderPassDesc.Subpasses[subpassIndex];
            auto& dxSubpass = renderPassDX12->Subpasses[subpassIndex];
            dxSubpass.RenderTargets.resize(subpass.ColorAttachmentCount);
            for (size_t attachmentIndex = 0; attachmentIndex < subpass.ColorAttachmentCount; attachmentIndex++)
            {
                auto& subpassColorAttachment = subpass.ColorAttachments[attachmentIndex];
                auto& colorAttachment = renderPassDesc.RenderTargets[subpassColorAttachment.Attachment];
                auto& dxColorAttachment = dxSubpass.RenderTargets[attachmentIndex];

                // Properties to be set via render pass begin:
                //    dxColorAttachment.BeginningAccess.Clear.ClearValue.Color
                //    dxColorAttachment.cpuDescriptor

                dxColorAttachment.BeginningAccess.Clear.ClearValue.Format = Translate(colorAttachment.Format);
                dxColorAttachment.BeginningAccess.Type = Translate(colorAttachment.BeginningAccess);
                // TODO: Add support for multisample resolve textures
                dxColorAttachment.EndingAccess.Resolve = {};
                dxColorAttachment.EndingAccess.Type = Translate(colorAttachment.EndingAccess);
            }

            if (renderPassDesc.DepthStencil != nullptr)
            {
                D3D12_RENDER_PASS_DEPTH_STENCIL_DESC depthStencilDesc;

                // Properties to be set via render pass begin:
                //    depthStencilDesc.cpuDescriptor
                //    depthStencilDesc.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Depth
                //    depthStencilDesc.StencilBeginningAccess.Clear.ClearValue.DepthStencil.Stencil

                depthStencilDesc.DepthBeginningAccess.Clear.ClearValue.Format = Translate(renderPassDesc.DepthStencil->Format);
                depthStencilDesc.DepthBeginningAccess.Type = Translate(renderPassDesc.DepthStencil->DepthBeginningAccess);
                depthStencilDesc.DepthEndingAccess.Resolve = {};
                depthStencilDesc.DepthEndingAccess.Type = Translate(renderPassDesc.DepthStencil->DepthEndingAccess);
                depthStencilDesc.StencilBeginningAccess.Type = Translate(renderPassDesc.DepthStencil->StencilBeginningAccess);
                depthStencilDesc.StencilEndingAccess.Resolve = {};
                depthStencilDesc.StencilEndingAccess.Type = Translate(renderPassDesc.DepthStencil->StencilEndingAccess);
            }
            else
            {
                dxSubpass.DepthStencil.reset();
            }

            // TODO: Add support for DX render pass flags
            dxSubpass.Flags = D3D12_RENDER_PASS_FLAG_NONE;
        }
    }

    void DeviceManagerDX12::CheckTearingSupport()
    {
        ComPtr<IDXGIFactory6> factory;
        HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
        BOOL allowTearing = FALSE;
        if (SUCCEEDED(hr))
        {
            hr = factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
        }

        m_tearingSupport = SUCCEEDED(hr) && allowTearing;
    }

    DXGI_FORMAT DeviceManagerDX12::Translate(ImageFormat imageFormat)
    {
        // The ImageFormat enum is based on D3D so a direct translation is valid.
        return static_cast<DXGI_FORMAT>(imageFormat);
    }

    void DeviceManagerDX12::GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter)
    {
        *ppAdapter = nullptr;

        ComPtr<IDXGIAdapter1> adapter;

        ComPtr<IDXGIFactory6> factory6;

        if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
        {
            for (
                UINT adapterIndex = 0;
                SUCCEEDED(factory6->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)));
                ++adapterIndex
                )
            {
                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);

                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    continue;
                }

                if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_ACTIVE_FEATURE_LEVEL, _uuidof(ID3D12Device), nullptr)))
                {
                    D_LOG_INFO("D3D12 device selected: {}", CharArrToStr(desc.Description, 128));
                    break;
                }
            }
        }

        *ppAdapter = adapter.Detach();
    }

    inline int ComputeIntersectionArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
    {
        return max(0, min(ax2, bx2) - max(ax1, bx1)) * max(0, min(ay2, by2) - max(ay1, by1));
    }

    void DeviceManagerDX12::PrintDisplayColorSpaceInfo(SDL_Window* window)
    {
        if (m_dxgiFactory->IsCurrent() == false)
        {
            ThrowIfFailed(
                CreateDXGIFactory2(0, IID_PPV_ARGS(&m_dxgiFactory))
            );
        }

        ComPtr<IDXGIAdapter1> dxgiAdapter;
        ThrowIfFailed(m_dxgiFactory->EnumAdapters1(0, &dxgiAdapter));

        UINT i = 0;
        ComPtr<IDXGIOutput> currentOutput;
        ComPtr<IDXGIOutput> bestOutput;
        float bestIntersectArea = -1;

        // Get the window bounds
        int ax1 = 0;
        int ay1 = 0;
        int ax2 = 0;
        int ay2 = 0;
        SDL_GetWindowPosition(window, &ax1, &ay1);
        SDL_GetWindowSize(window, &ax2, &ay2);
        ax2 += ax1;
        ay2 += ay1;

        while (dxgiAdapter->EnumOutputs(i, &currentOutput) != DXGI_ERROR_NOT_FOUND)
        {
            DXGI_OUTPUT_DESC desc;
            ThrowIfFailed(currentOutput->GetDesc(&desc));
            RECT r = desc.DesktopCoordinates;
            int bx1 = r.left;
            int by1 = r.top;
            int bx2 = r.right;
            int by2 = r.bottom;

            int intersectArea = ComputeIntersectionArea(ax1, ay1, ax2, ay2, bx1, by1, bx2, by2);
            if (intersectArea > bestIntersectArea)
            {
                bestOutput = currentOutput;
                bestIntersectArea = static_cast<float>(intersectArea);
            }

            i++;
        }

        ComPtr<IDXGIOutput6> output6;
        ThrowIfFailed(bestOutput.As(&output6));

        DXGI_OUTPUT_DESC1 desc1;
        ThrowIfFailed(output6->GetDesc1(&desc1));

        std::string rotation = "UNDEFINED";
        if (desc1.Rotation >= 0) {
            rotation = fmt::format("{} degrees", (desc1.Rotation - 1) * 90);
        }

        D_LOG_INFO(
            "\nActive Display Info:\n"
            "  Device Name: {}\n"
            "  Bits Per Color: {}\n"
            "  Max Luminance: {}\n"
            "  Min Luminance: {}\n"
            "  Max Full Frame Luminance: {}\n"
            "  Rotation: {}\n"
            "  White Point: [{}, {}]\n"
            "  R Primary: [{}, {}]\n"
            "  G Primary: [{}, {}]\n"
            "  B Primary: [{}, {}]",
            CharArrToStr(desc1.DeviceName, 128),
            desc1.BitsPerColor,
            desc1.MaxLuminance,
            desc1.MinLuminance,
            desc1.MaxFullFrameLuminance,
            rotation,
            desc1.WhitePoint[0], desc1.WhitePoint[1],
            desc1.RedPrimary[0], desc1.RedPrimary[1],
            desc1.GreenPrimary[0], desc1.GreenPrimary[1],
            desc1.BluePrimary[0], desc1.BluePrimary[1]
        );
    }
}
