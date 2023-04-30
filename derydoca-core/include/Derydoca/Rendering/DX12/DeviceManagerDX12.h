#pragma once

#include <algorithm>

#define NOMINMAX
#include <Windows.h>
#include <directx/d3d12.h>
#include <dxgi1_5.h>
#include <dxgidebug.h>
#include <wrl/client.h>

#include <nvrhi/d3d12.h>
#include <nvrhi/validation.h>

#include "Derydoca/DeviceManager.h"
#include "Derydoca/Rendering/DX12/RenderPassDX12.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

using nvrhi::RefCountPtr;

namespace Derydoca::Rendering
{
	class DeviceManagerDX12 : public DeviceManager
	{
	public:
		static const UINT FrameCount = 2;

		DeviceManagerDX12() = default;
		DeviceManagerDX12(const DeviceManagerDX12&) = delete;

		void Render() override;
		void CreateRenderPass(const RenderPassDesc& renderPassDesc, RenderPass* renderPass) override;
		void CreateCommandBuffer(CommandBuffer* commandBuffer) const override;

	protected:
		bool CreateDeviceAndSwapChain() override;
		void DestroyDeviceAndSwapChain() override;
		nvrhi::GraphicsAPI GetGraphicsAPI() const override;
		uint32_t GetBackBufferCount() override;
		void ResizeSwapChain() override;
		nvrhi::ITexture* GetBackBuffer(uint32_t index) override;

	private:
		RefCountPtr<IDXGIAdapter> GetHardwareAdapter(const std::wstring& targetName);
		bool CreateRenderTargets();
		void ReleaseRenderTargets();

		static bool MoveWindowOntoAdapter(IDXGIAdapter* targetAdapter, RECT& rect);

		RefCountPtr<ID3D12Device> m_Device12;
		std::vector<RefCountPtr<ID3D12Resource>> m_SwapChainBuffers;
		std::vector<nvrhi::TextureHandle> m_RhiSwapChainBuffers;
		ComPtr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];
		RefCountPtr<ID3D12CommandQueue> m_GraphicsQueue;
		RefCountPtr<ID3D12CommandQueue> m_ComputeQueue;
		RefCountPtr<ID3D12CommandQueue> m_CopyQueue;
		RefCountPtr<IDXGIFactory> m_dxgiFactory;
		ComPtr<IDXGISwapChain3> m_SwapChain;
		ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		ComPtr<ID3D12GraphicsCommandList4> m_commandList;
		UINT m_rtvDescriptorSize;
		RenderPassDX12 m_renderPass;
		DXGI_SWAP_CHAIN_DESC1 m_SwapChainDesc{};

		UINT m_frameIndex;
		std::vector<HANDLE> m_FrameFenceEvents;
		RefCountPtr<ID3D12Fence> m_FrameFence;
		UINT64 m_fenceValues[FrameCount];
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC m_FullScreenDesc{};

		std::string m_RendererString;
		HWND m_hWnd;
		bool m_TearingSupported = false;
		RefCountPtr<IDXGIAdapter> m_DxgiAdapter;

		// Whether or not tearing is available for fullscreen borderless windowed mode.
		bool m_tearingSupport;
	};
}
