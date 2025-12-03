#pragma once

#include "Vec2.h"

namespace Direction
{
    inline const Vec2 UP{ -1, 0 };
    inline const Vec2 DOWN{ 1, 0 };
    inline const Vec2 LEFT{ 0, -1 };
    inline const Vec2 RIGHT{ 0, 1 };

    inline bool areOpposite(const Vec2& dir1, const Vec2& dir2)
    {
        return dir1 == -dir2;
    }
}