#include <stdio.h>
#include "Editor/CommandLineArgs.h"
#include "Logging/Log.h"
#include "DerydocaEngine/DeviceManager.h"

const char* WindowName = "Derydoca Engine";

int main(int argc, char* argv[])
{
    DerydocaEngine::Logging::Log::Init();
    D_LOG_TRACE("Engine startup");

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
