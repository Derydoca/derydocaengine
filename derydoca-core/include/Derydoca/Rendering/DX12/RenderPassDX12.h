#pragma once

#include <d3d12.h>
#include <cstdint>
#include <optional>

namespace Derydoca::Rendering
{
    class RenderSubpassDX12
    {
    public:
        std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> RenderTargets;
        std::optional<D3D12_RENDER_PASS_DEPTH_STENCIL_DESC> DepthStencil;
        D3D12_RENDER_PASS_FLAGS Flags;
    };

    class RenderPassDX12
    {
    public:
        std::vector<RenderSubpassDX12> Subpasses;
    };
}
