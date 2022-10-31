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

    auto deviceManagerSettings = DeviceManagerSettings();

    auto window = SDL_CreateWindow("Derydoca Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, deviceManagerSettings.width, deviceManagerSettings.height, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        D_LOG_CRITICAL("Unable to create the main window!\nError: {}", SDL_GetError());
        return -1;
    }

    auto renderingAPI = DerydocaEngine::DeviceManager::Create(RenderingAPI::Direct3D12, deviceManagerSettings, window);
    if (renderingAPI == nullptr)
    {
        D_LOG_CRITICAL("Unable to create device manager!");
        return -1;
    }

    while (true)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                break;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    break;
                }
            }
        }
        renderingAPI->Render();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
