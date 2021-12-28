#include <stdio.h>
#include "Editor/CommandLineArgs.h"
#include "Editor/EditorWindow.h"
#include "Logging/Log.h"

const char* WindowName = "Derydoca Engine";

int main(int argc, char* argv[])
{
    DerydocaEngine::Logging::Log::Init();
    D_LOG_TRACE("Engine startup");

    DerydocaEngine::Editor::UI::EditorWindow editorWindow;

    int returnCode = 0;

    auto editorWindowSettings = DerydocaEngine::Editor::UI::EditorWindowSettings();
    editorWindowSettings.BackbufferWidth = 800;
    editorWindowSettings.BackbufferHeight = 600;
    editorWindowSettings.SwapChainSampleCount = 1;
    editorWindowSettings.RefreshRate = 60;
    returnCode = editorWindow.CreateWindowAndSwapChain(editorWindowSettings, "Derydoca Editor");
    if (returnCode != 0)
    {
        return returnCode;
    }

    returnCode = editorWindow.RunUpdateLoop();

    editorWindow.Cleanup();
    D_LOG_TRACE("Engine shut-down");

    return returnCode;
}
