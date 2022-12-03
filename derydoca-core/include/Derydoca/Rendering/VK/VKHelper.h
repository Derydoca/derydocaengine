#pragma once
#include "Derydoca/Core.h"

inline void ThrowIfFailed(VkResult result)
{
    if (result < 0)
    {
        D_LOG_CRITICAL("VKFailure: {0:x}", result);
        exit(-1);
    }
}
