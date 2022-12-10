#include <stdio.h>
#include <SDL2/SDL.h>
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

    SDL_Init(SDL_INIT_EVERYTHING);

    Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (renderingAPI == Rendering::RenderingAPI::Vulkan)
    {
        windowFlags |= SDL_WINDOW_VULKAN;
    }
    auto window = SDL_CreateWindow("Derydoca Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, deviceManagerSettings.width, deviceManagerSettings.height, windowFlags);
    if (window == NULL)
    {
        D_LOG_CRITICAL("Unable to create the main window!\nError: {}", SDL_GetError());
        return -1;
    }

    auto deviceManager = std::unique_ptr<DeviceManager>(DeviceManager::Create(renderingAPI, deviceManagerSettings, window));
    if (deviceManager == nullptr)
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
            else if (event.type == SDL_WINDOWEVENT)
            {
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    deviceManager->SignalWindowResizedEvent();
                    break;
                }
            }
        }
        deviceManager->Render();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
