#pragma once
#include <cstdint>

namespace Derydoca
{
    template<typename T>
    struct Vector2
    {
        T x;
        T y;
    };

    template<typename T>
    struct Rect
    {
        Vector2<T> offset;
        Vector2<T> extent;
    };

    typedef Vector2<int> int2;

    typedef Rect<int32_t> RectI;
    typedef Rect<uint32_t> RectUI;

    struct Viewport
    {
        float x;
        float y;
        float width;
        float height;
        float minDepth;
        float maxDepth;
    };
}