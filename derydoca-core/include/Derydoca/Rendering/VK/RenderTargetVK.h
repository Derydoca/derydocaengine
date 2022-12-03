#pragma once

#include <vulkan/vulkan.h>

#include "Derydoca/Rendering/RenderTarget.h"

namespace Derydoca::Rendering
{
    class RenderTargetVK : public RenderTarget
    {
    public:
        VkFramebuffer* Framebuffer;
    };
}