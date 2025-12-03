#pragma once

#include <utility>

struct UVec2
{
    unsigned char x;
    unsigned char y;

    bool operator==(const UVec2& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct Vec2
{
    int x;
    int y;

    bool operator==(const Vec2& other) const
    {
        return x == other.x && y == other.y;
    }

    Vec2 operator-() const { return { -x, -y }; }
};