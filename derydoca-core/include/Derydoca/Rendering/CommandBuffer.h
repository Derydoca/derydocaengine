#pragma once
#include "Derydoca/Core.h"
#include "Derydoca/Rendering/Common.h"

namespace Derydoca::Rendering
{
    class CommandBuffer
    {
        virtual void SetViewport(Viewport viewport) = 0;
        virtual void SetScissorRect(RectI rect) = 0;
        virtual void BeginRenderPass(RenderPass* renderPass, RenderPassBeginInfo& beginInfo) = 0;
        virtual void NextSubpass() = 0;
        virtual void EndRenderPass() = 0;
    };
}
