#include <stdio.h>
#include "Derydoca/Editor/CommandLineArgs.h"
#include "Derydoca/Logging/Log.h"
#include "Derydoca/DeviceManager.h"

const char* WindowName = "Derydoca Engine";


int main(int argc, const char* argv[])
{
    DerydocaEngine::Logging::Log::Init();
    D_LOG_TRACE("Engine startup");

    return 0;
}
