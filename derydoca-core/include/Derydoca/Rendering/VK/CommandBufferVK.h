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
        CommandBufferVK(VkCommandBuffer* internalCommandBuffer);

        void SetViewport(Viewport viewport) override;
        void SetScissorRect(RectI rect) override;
        void BeginRenderPass(RenderPass* renderPass, RenderPassBeginInfo& beginInfo) override;
        void NextSubpass() override;
        void EndRenderPass() override;

    private:
        VkCommandBuffer* commandBuffer;
        VkRenderPass* activeRenderPass;

    };
}
