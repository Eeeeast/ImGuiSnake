#include "Field2.h"
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <algorithm>

Field2::Field2(unsigned char width, unsigned char height, char defaultCellFiller)
    : m_size{ height, width }  // x = высота, y = ширина
    , m_cellFiller(defaultCellFiller)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    m_freePositions.reserve(static_cast<size_t>(m_size.x) * m_size.y);
    clear();
}

void Field2::clear()
{
    m_board.assign(m_size.x, std::vector<char>(m_size.y, m_cellFiller));

    m_freePositions.clear();
    for (unsigned char row = 0; row < m_size.x; ++row)
        for (unsigned char col = 0; col < m_size.y; ++col)
            m_freePositions.push_back({ row, col });
}

std::optional<UVec2> Field2::tryFindFreeRandomPos() const
{
    if (m_freePositions.empty())
        return std::nullopt;
    return m_freePositions[std::rand() % m_freePositions.size()];
}

void Field2::setCell(UVec2 pos, char symbol)
{
    assert(pos.x < m_size.x && pos.y < m_size.y);
    m_board[pos.x][pos.y] = symbol;

    auto it = std::remove(m_freePositions.begin(), m_freePositions.end(), pos);
    m_freePositions.erase(it, m_freePositions.end());

    if (symbol == m_cellFiller)
        m_freePositions.push_back(pos);
}

UVec2 Field2::getShiftedPosition(UVec2 pos, Vec2 shift) const
{
    return
    {
        static_cast<unsigned char>((static_cast<int>(pos.x) + shift.x + m_size.x) % m_size.x),
        static_cast<unsigned char>((static_cast<int>(pos.y) + shift.y + m_size.y) % m_size.y)
    };
}

char Field2::getCell(UVec2 pos) const
{
    assert(pos.x < m_size.x && pos.y < m_size.y);
    return m_board[pos.x][pos.y];
}

std::optional<UVec2> Field2::tryGetCenterPos() const
{
    if (m_size.x == 0 || m_size.y == 0)
        return std::nullopt;
    return UVec2{ (unsigned char)(m_size.x / 2), (unsigned char)(m_size.y / 2) };
}