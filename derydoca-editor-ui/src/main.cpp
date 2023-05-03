#include <stdio.h>
#undef main
#include <memory>

#include "Derydoca/Editor/CommandLineArgs.h"
#include "Derydoca/Logging/Log.h"
#include "Derydoca/DeviceManager.h"

const char* WindowName = "Derydoca Engine";

using namespace Derydoca;
using namespace Derydoca::Rendering;

class Example : public IRenderPass
{
public:
    using IRenderPass::IRenderPass;

    bool Init();
};

#pragma region Example_Impl
bool Example::Init()
{
    return true;
}
#pragma endregion Example_Impl

int main(int argc, const char* argv[])
{
    Derydoca::Logging::Log::Init();
    D_LOG_TRACE("Engine startup");

    auto renderingAPI = Rendering::RenderingAPI::Direct3D12;

    DeviceManager* deviceManager = DeviceManager::Create(renderingAPI);
    if (deviceManager == nullptr)
    {
        D_LOG_CRITICAL("Unable to create device manager!");
        return -1;
    }

    auto deviceParams = Rendering::DeviceCreationParams();
#if _DEBUG
    deviceParams.enableDebugRuntime = true;
    deviceParams.enableNvrhiValidationLayer = true;
#endif

    if (!deviceManager->CreateWindowDeviceAndSwapChain(deviceParams, "Derydoca Engine"))
    {
        D_LOG_CRITICAL("Cannot initialize a graphics device with the requested parameters.");
        return -1;
    }

    // TODO: Insert render loop

    Example example(deviceManager);
    if (example.Init())
    {
        deviceManager->AddRenderPassToBack(&example);
        deviceManager->RunMessageLoop();
        deviceManager->RemoveRenderPass(&example);
    }

    deviceManager->Shutdown();

    return 0;
}
