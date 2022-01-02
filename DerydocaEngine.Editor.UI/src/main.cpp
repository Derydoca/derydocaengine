#include <stdio.h>
#include "DerydocaEngine/Editor/CommandLineArgs.h"
#include "DerydocaEngine/Logging/Log.h"
#include "DerydocaEngine/DeviceManager.h"

const char* WindowName = "Derydoca Engine";

nvrhi::GraphicsAPI GetGraphicsAPIFromArgs(DerydocaEngine::Editor::CommandLineArgs* args)
{
    const std::string gapiKey = "gapi";
    const std::string VulkanValue = "VK";
    const std::string D3D12Value = "D3D12";

#ifdef USE_VULKAN
    auto defaultGapi = nvrhi::GraphicsAPI::VULKAN;
    const char* defaultGapiName = "Vulkan";
#elif USE_DX12
    auto defaultGapi = nvrhi::GraphicsAPI::D3D12;
    const char* defaultGapiName = "D3D12";
#else
    auto defaultGapi = (nvrhi::GraphicsAPI)(-1);
    const char* defaultGapiName = "NULL";
    D_LOG_CRITICAL("The engine was compiled without a valid graphics API. Unable to continue!");
#endif

    if (!args->KeyExists(gapiKey))
    {
        D_LOG_INFO("No graphics api was specified. Defaulting to {0}.", defaultGapiName);
        return defaultGapi;
    }

    auto values = args->GetValues(gapiKey);
    if (values.size() == 0)
    {
        D_LOG_WARN("The command line argument '{0}' had no specified value. Defaulting to {1}.", gapiKey, defaultGapiName);
        return defaultGapi;
    }

    std::string gapiValue = values[0];
#ifdef USE_VULKAN
    if (gapiValue == VulkanValue)
    {
        D_LOG_TRACE("Graphics API set to Vulkan.");
        return nvrhi::GraphicsAPI::VULKAN;
    }
#endif
#ifdef USE_DX12
    if (gapiValue == D3D12Value)
    {
        D_LOG_TRACE("Graphics API set to D3D12.");
        return nvrhi::GraphicsAPI::D3D12;
    }
#endif

    D_LOG_WARN("The command line argument '{0}' had an invalid value of '{1}'. Defaulting to {2}.", gapiKey, gapiValue, defaultGapiName);
    return defaultGapi;
}

int main(int argc, const char* argv[])
{
    DerydocaEngine::Logging::Log::Init();
    D_LOG_TRACE("Engine startup");

    auto commandLineArgs = new DerydocaEngine::Editor::CommandLineArgs(argc, argv);
    nvrhi::GraphicsAPI graphicsApi = GetGraphicsAPIFromArgs(commandLineArgs);

    auto deviceManager = DerydocaEngine::DeviceManager::Create(graphicsApi);
    if (!deviceManager)
    {
        D_LOG_CRITICAL("Unable to create the device manager!");
        return -2;
    }

    int returnCode = 0;

    auto editorWindowSettings = DerydocaEngine::DeviceManagerSettings();
    editorWindowSettings.BackbufferWidth = 800;
    editorWindowSettings.BackbufferHeight = 600;
    editorWindowSettings.SwapChainSampleCount = 1;
    editorWindowSettings.RefreshRate = 60;

#ifdef _DEBUG
    editorWindowSettings.EnableDebugRuntime = true;
    editorWindowSettings.EnableValidationLayer = true;
#endif

    returnCode = deviceManager->CreateWindowAndSwapChain(editorWindowSettings, "Derydoca Editor");
    if (returnCode != 0)
    {
        return returnCode;
    }

    returnCode = deviceManager->RunUpdateLoop();

    D_LOG_TRACE("Engine shutdown");
    deviceManager->Cleanup();
    delete(deviceManager);

    return returnCode;
}
