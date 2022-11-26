#pragma once
#include "Derydoca/Core.h"

namespace Derydoca::Rendering
{
    enum class RenderPassBeginningAccess
    {
        Discard = 0,
        Preserve,
        Clear,
        NoAccess
    };

    enum class RenderPassEndingAccess
    {
        Discard = 0,
        Preserve,
        Resolve,
        NoAccess
    };

    class CommandBuffer
    {
        virtual void SetViewport(Viewport viewport) = 0;
        virtual void SetScissorRect(RectI rect) = 0;
        virtual void BeginRenderPass() = 0;
    };
}
