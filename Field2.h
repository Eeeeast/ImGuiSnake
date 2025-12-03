#pragma once

#include "Vec2.h"
#include <vector>
#include <optional>
#include <cstddef>

class Field2
{
public:
    Field2(unsigned char width, unsigned char height, char defaultCellFiller);

    void clear();
    std::optional<UVec2> tryFindFreeRandomPos() const;
    void setCell(UVec2 pos, char symbol);

    unsigned char getWidth() const { return m_size.y; }
    unsigned char getHeight() const { return m_size.x; }
    size_t getFreePosCount() const { return m_freePositions.size(); }
    UVec2 getShiftedPosition(UVec2 pos, Vec2 shift) const;
    char getCell(UVec2 pos) const;
    std::optional<UVec2> tryGetCenterPos() const;

private:
    std::vector<std::vector<char>> m_board;
    std::vector<UVec2> m_freePositions;
    const UVec2 m_size;
    const char m_cellFiller;
};