#include "Derydoca/Rendering/VK/CommandBufferVK.h"

namespace Derydoca::Rendering
{
    void CommandBufferVK::SetViewport(Viewport viewport)
    {
        VkViewport vkViewport{};
        vkViewport.x = viewport.x;
        vkViewport.y = viewport.y;
        vkViewport.width = viewport.width;
        vkViewport.height = viewport.height;
        vkViewport.minDepth = viewport.minDepth;
        vkViewport.maxDepth = viewport.maxDepth;

        vkCmdSetViewport(commandBuffer, 0, 1, &vkViewport);
    }

    void CommandBufferVK::SetScissorRect(RectI rect)
    {
        VkRect2D vkRect{};
        vkRect.extent.width = rect.extent.x;
        vkRect.extent.height = rect.extent.y;
        vkRect.offset.x = rect.offset.x;
        vkRect.offset.y = rect.offset.y;

        vkCmdSetScissor(commandBuffer, 0, 1, &vkRect);
    }
}
