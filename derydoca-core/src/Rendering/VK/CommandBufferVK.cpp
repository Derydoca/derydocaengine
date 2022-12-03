#include <assert.h>
#include <vulkan/vulkan.h>

#include "Derydoca/Rendering/VK/CommandBufferVK.h"
#include "Derydoca/Rendering/VK/RenderTargetVK.h"
#include "Derydoca/Rendering/VK/RenderPassVK.h"

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

    // TODO: Move to VKHelper.h
    inline VkRect2D Convert(const RectI rect)
    {
        VkRect2D vkRect{};
        vkRect.extent.width = rect.extent.x;
        vkRect.extent.height = rect.extent.y;
        vkRect.offset.x = rect.offset.x;
        vkRect.offset.y = rect.offset.y;
        return vkRect;
    }

    void CommandBufferVK::BeginRenderPass(RenderPass* renderPass, RenderPassBeginInfo& beginInfo)
    {
        assert(activeRenderPass == nullptr);

        activeRenderPass = static_cast<VkRenderPass*>(renderPass);

        auto clearValues = std::vector<VkClearValue>(beginInfo.ClearValueCount);
        for (size_t i = 0; i < beginInfo.ClearValueCount; i++)
        {
            auto& clearValue = beginInfo.ClearValues[i];
            auto vkClearValue = VkClearValue{};

            vkClearValue.color.float32[0] = clearValue.Color.float32[0];
            vkClearValue.color.float32[1] = clearValue.Color.float32[1];
            vkClearValue.color.float32[2] = clearValue.Color.float32[2];
            vkClearValue.color.float32[3] = clearValue.Color.float32[3];

            vkClearValue.color.int32[0] = clearValue.Color.int32[0];
            vkClearValue.color.int32[1] = clearValue.Color.int32[1];
            vkClearValue.color.int32[2] = clearValue.Color.int32[2];
            vkClearValue.color.int32[3] = clearValue.Color.int32[3];

            vkClearValue.color.uint32[0] = clearValue.Color.uint32[0];
            vkClearValue.color.uint32[1] = clearValue.Color.uint32[1];
            vkClearValue.color.uint32[2] = clearValue.Color.uint32[2];
            vkClearValue.color.uint32[3] = clearValue.Color.uint32[3];

            vkClearValue.depthStencil.depth = clearValue.DepthStencil.Depth;
            vkClearValue.depthStencil.stencil = clearValue.DepthStencil.Stencil;

            clearValues.push_back(vkClearValue);
        }

        VkRenderPassBeginInfo renderPassBeginInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
        renderPassBeginInfo.renderPass = *activeRenderPass;
        renderPassBeginInfo.framebuffer = *static_cast<RenderTargetVK*>(beginInfo.Target)->Framebuffer;
        renderPassBeginInfo.clearValueCount = beginInfo.ClearValueCount;
        renderPassBeginInfo.pClearValues = clearValues.data();
        renderPassBeginInfo.renderArea = Convert(beginInfo.Area);

        vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void CommandBufferVK::NextSubpass()
    {
    }

    void CommandBufferVK::EndRenderPass()
    {
    }
}
