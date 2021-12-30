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

    if (!args->KeyExists(gapiKey))
    {
        D_LOG_INFO("No graphics api was specified. Defaulting to Vulkan.");
        return nvrhi::GraphicsAPI::VULKAN;
    }

    auto values = args->GetValues(gapiKey);
    if (values.size() == 0)
    {
        D_LOG_WARN("The command line argument '{0}' had no specified value. Defaulting to Vulkan.", gapiKey);
        return nvrhi::GraphicsAPI::VULKAN;
    }

    std::string gapiValue = values[0];
    if (gapiValue == VulkanValue)
    {
        D_LOG_TRACE("Graphics API set to Vulkan.");
        return nvrhi::GraphicsAPI::VULKAN;
    }
    else if (gapiValue == D3D12Value)
    {
        D_LOG_TRACE("Graphics API set to D3D12.");
        return nvrhi::GraphicsAPI::D3D12;
    }

    D_LOG_WARN("The command line argument '{0}' had an invalid value of '{1}'. Defaulting to Vulkan.", gapiKey, gapiValue);
    return nvrhi::GraphicsAPI::VULKAN;
}

int main(int argc, const char* argv[])
{
    DerydocaEngine::Logging::Log::Init();
    D_LOG_TRACE("Engine startup");

    auto commandLineArgs = new DerydocaEngine::Editor::CommandLineArgs(argc, argv);
    nvrhi::GraphicsAPI graphicsApi = GetGraphicsAPIFromArgs(commandLineArgs);

    auto deviceManager = DerydocaEngine::DeviceManager();

    int returnCode = 0;

    auto editorWindowSettings = DerydocaEngine::DeviceManagerSettings();
    editorWindowSettings.BackbufferWidth = 800;
    editorWindowSettings.BackbufferHeight = 600;
    editorWindowSettings.SwapChainSampleCount = 1;
    editorWindowSettings.RefreshRate = 60;
    returnCode = deviceManager.CreateWindowAndSwapChain(editorWindowSettings, "Derydoca Editor");
    if (returnCode != 0)
    {
        return returnCode;
    }

    returnCode = deviceManager.RunUpdateLoop();

    deviceManager.Cleanup();
    D_LOG_TRACE("Engine shut-down");

    return returnCode;
}
