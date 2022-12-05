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

    template<typename TO, typename TE>
    struct Rect
    {
        Vector2<TO> offset;
        Vector2<TE> extent;
    };

    typedef Vector2<int> int2;

    typedef Rect<int32_t, uint32_t> RectI;

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