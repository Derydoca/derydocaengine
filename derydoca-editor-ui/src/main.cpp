#include <stdio.h>
#include <SDL2/SDL.h>
#undef main

#include "Derydoca/Editor/CommandLineArgs.h"
#include "Derydoca/Logging/Log.h"
#include "Derydoca/DeviceManager.h"

const char* WindowName = "Derydoca Engine";

using namespace DerydocaEngine;

int main(int argc, const char* argv[])
{
    DerydocaEngine::Logging::Log::Init();
    D_LOG_TRACE("Engine startup");

    auto window = SDL_CreateWindow("Derydoca Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        D_LOG_CRITICAL("Unable to create the main window!\nError: {}", SDL_GetError());
        return -1;
    }

    auto renderingAPI = DerydocaEngine::DeviceManager::Create(RenderingAPI::Direct3D12);
    if (renderingAPI == nullptr)
    {
        D_LOG_CRITICAL("Unable to create device manager!");
        return -1;
    }

    SDL_Delay(2000);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
