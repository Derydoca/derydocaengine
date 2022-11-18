#pragma once
#include "Derydoca/Rendering/CommandBuffer.h"

#include <vulkan/vulkan.h>

#include "Derydoca/Logging/Log.h"
#include "Derydoca/Rendering/VK/VKHelper.h"

namespace Derydoca::Rendering
{
    class CommandBufferVK : public CommandBuffer
    {
    public:
        void SetViewport(Viewport viewport) override;
        void SetScissorRect(RectI rect);
        void Transition();

    private:
        VkCommandBuffer commandBuffer;

    };
}
