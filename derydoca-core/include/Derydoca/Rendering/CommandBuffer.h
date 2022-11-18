#pragma once
#include "Derydoca/Core.h"

namespace Derydoca::Rendering
{
    class CommandBuffer
    {
        virtual void SetViewport(Viewport viewport) = 0;
    };
}
