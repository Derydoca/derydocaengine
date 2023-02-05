#include "Derydoca/Rendering/VK/VulkanBackend.h"

namespace Derydoca::Rendering::vk
{
    Queue::Queue(const DeviceManagerVK& deviceManager, VkQueue queue, uint32_t familyIndex)
        : m_deviceManager(deviceManager)
        , m_queue(queue)
        , m_familyIndex(familyIndex)
        , m_semaphore()
    {
        // TODO: Create semaphore from m_deviceManager
        //m_deviceManager.Render();
    }

    Queue::~Queue()
    {
        // TODO: Destroy semaphore
    }

    CommandBufferVK Queue::CreateCommandBuffer()
    {
        return nullptr;
    }
}