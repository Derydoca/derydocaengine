#pragma once
#include "DeviceManagerVK.h"
#include "Derydoca/Rendering/VK/DeviceManagerVK.h"
#include "Derydoca/Rendering/VK/CommandBufferVK.h"

#include <vulkan/vulkan.h>

namespace Derydoca::Rendering::vk
{
    class Queue
    {
    public:
        Queue(const DeviceManagerVK& deviceManager, VkQueue queue, uint32_t familyIndex);
        ~Queue();

        CommandBufferVK CreateCommandBuffer();

    private:
        const DeviceManagerVK& m_deviceManager;
        VkSemaphore m_semaphore;
        VkQueue m_queue;
        uint32_t m_familyIndex;
    };
}