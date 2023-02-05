#pragma once

#include <algorithm>

#include <directx/d3d12.h>
#include <directx/d3dx12.h>
#include <Windows.h>
#include <dxgi1_5.h>
#include <dxgidebug.h>
#include <wrl/client.h>
#include <dxgi1_6.h>

#include "Derydoca/DeviceManager.h"
#include "Derydoca/Rendering/DX12/RenderPassDX12.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

namespace Derydoca::Rendering
{
	class DeviceManagerDX12 : public DeviceManager
	{
	public:
		static const UINT FrameCount = 2;

		DeviceManagerDX12(const DeviceManagerSettings& settings, SDL_Window* sdlWindow);
		DeviceManagerDX12(const DeviceManagerDX12&) = delete;

		void Render() override;
		void CreateRenderPass(const RenderPassDesc& renderPassDesc, RenderPass* renderPass) override;
		void CreateCommandBuffer(CommandBuffer* commandBuffer) const override;

	private:
		DIRECT_ENUM_TRANSLATE_FUNCS(RenderPassBeginningAccess, D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE);
		DIRECT_ENUM_TRANSLATE_FUNCS(RenderPassEndingAccess, D3D12_RENDER_PASS_ENDING_ACCESS_TYPE);

		DXGI_FORMAT Translate(ImageFormat imageFormat);

		void Initialize(const DeviceManagerSettings& settings);
		void GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter);
		void CheckTearingSupport();
		void PrintDisplayColorSpaceInfo(SDL_Window* window);

		ComPtr<ID3D12Device> m_device;
		CD3DX12_CPU_DESCRIPTOR_HANDLE m_renderTargetHandles[FrameCount];
		ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
		ComPtr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];
		ComPtr<ID3D12CommandQueue> m_renderingCommandQueue;
		ComPtr<IDXGIFactory4> m_dxgiFactory;
		ComPtr<IDXGISwapChain4> m_swapChain;
		ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		ComPtr<ID3D12GraphicsCommandList4> m_commandList;
		UINT m_rtvDescriptorSize;
		RenderPassDX12 m_renderPass;

		UINT m_frameIndex;
		HANDLE m_fenceEvents[FrameCount];
		ComPtr<ID3D12Fence> m_fences[FrameCount];
		UINT64 m_fenceValues[FrameCount];

		// Whether or not tearing is available for fullscreen borderless windowed mode.
		bool m_tearingSupport;
	};
}
