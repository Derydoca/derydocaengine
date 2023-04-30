#include <stdio.h>
#undef main
#include <memory>

#include "Derydoca/Editor/CommandLineArgs.h"
#include "Derydoca/Logging/Log.h"
#include "Derydoca/DeviceManager.h"

const char* WindowName = "Derydoca Engine";

using namespace Derydoca;
using namespace Derydoca::Rendering;

int main(int argc, const char* argv[])
{
    Derydoca::Logging::Log::Init();
    D_LOG_TRACE("Engine startup");

    auto deviceManagerSettings = Rendering::DeviceManagerSettings();
    auto renderingAPI = Rendering::RenderingAPI::Direct3D12;

    auto deviceManager = std::unique_ptr<DeviceManager>(DeviceManager::Create(renderingAPI, deviceManagerSettings));
    if (deviceManager == nullptr)
    {
        D_LOG_CRITICAL("Unable to create device manager!");
        return -1;
    }

    if (!deviceManager->CreateWindowDeviceAndSwapChain(deviceManagerSettings, "Derydoca Engine"))
    {
        D_LOG_CRITICAL("Cannot initialize a graphics device with the requested parameters.");
        return -1;
    }

    return 0;
}
