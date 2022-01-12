#pragma once

#include <algorithm>

#define NOMINMAX
#include <Windows.h>
#include <dxgi1_5.h>
#include <dxgidebug.h>

#include <nvrhi/d3d12.h>
#include <nvrhi/validation.h>

#include "DerydocaEngine/DeviceManager.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

namespace DerydocaEngine::Rendering
{
	class DeviceManagerDX12 : public DeviceManager
	{
	public:
		nvrhi::IDevice* GetDevice() const override
		{
			return m_NvrhiDevice;
		}

	protected:
		bool CreateDeviceAndSwapChain() override;
		void DestroyDeviceAndSwapChain() override;
		void Present() override;

	private:
		bool CreateRenderTargets();

	private:
		nvrhi::RefCountPtr<ID3D12Device> m_Device12;
		nvrhi::RefCountPtr<IDXGIAdapter> m_DxgiAdapter;
		nvrhi::RefCountPtr<ID3D12CommandQueue> m_GraphicsQueue;
		nvrhi::RefCountPtr<ID3D12CommandQueue> m_ComputeQueue;
		nvrhi::RefCountPtr<ID3D12CommandQueue> m_CopyQueue;
		nvrhi::RefCountPtr<IDXGISwapChain3> m_SwapChain;
		DXGI_SWAP_CHAIN_DESC1 m_SwapChainDesc{};
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC m_FullScreenDesc{};
		HWND m_hWnd;
		bool m_TearingSupported = false;
		std::vector<nvrhi::RefCountPtr<ID3D12Resource>> m_SwapChainBuffers;
		std::vector<nvrhi::TextureHandle> m_RhiSwapChainBuffers;
		nvrhi::RefCountPtr<ID3D12Fence> m_FrameFence;
		std::vector<HANDLE> m_FrameFenceEvents;
		UINT64 m_FrameCount = 1;
		nvrhi::DeviceHandle m_NvrhiDevice;

	};
}
