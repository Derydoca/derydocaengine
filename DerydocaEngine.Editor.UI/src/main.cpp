#include <SDL.h>
#include <SDL_main.h>
#include <stdio.h>
#include "Editor/CommandLineArgs.h"
#include "Editor/EditorWindow.h"

const char* WindowName = "Derydoca Engine";

int main(int argc, char* argv[])
{
    DerydocaEngine::Editor::UI::EditorWindow editorWindow;

    int returnCode = 0;

    returnCode = editorWindow.CreateWindowAndSwapChain();
    if (returnCode != 0)
    {
        return returnCode;
    }

    returnCode = editorWindow.RunUpdateLoop();

    editorWindow.Cleanup();

    return returnCode;
}
