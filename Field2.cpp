#include "Field2.h"
#include <ctime>
#include <cassert>

Field2::Field2(const unsigned char width, const unsigned char height, char defaultCellFiller)
	: size(height, width),
	cellFiller(defaultCellFiller)
{
	std::srand((unsigned int)std::time({}));
	freePositions.reserve((size_t)size.first * size.second);
	Clear();
}

void Field2::Clear()
{
	board = { size.first, std::vector<char>(size.second, cellFiller) };

	freePositions.clear();
	for (unsigned char row = 0; row < size.first; row++)
		for (unsigned char col = 0; col < size.second; col++)
			freePositions.push_back({ row, col });
}

std::optional<UVec2> Field2::TryFindFreeRandomPos() const
{
	if (freePositions.empty())
		return std::nullopt;
	return freePositions[std::rand() % freePositions.size()];
}

void Field2::SetCell(UVec2 pos, char symbol)
{
	assert(pos.first < size.first);
	assert(pos.second < size.second);
	board[pos.first][pos.second] = symbol;

	auto it = std::remove(freePositions.begin(), freePositions.end(), pos);
	freePositions.erase(it, freePositions.end());

	if (symbol == cellFiller)
		freePositions.push_back(pos);
}

UVec2 Field2::GetShiftedPosition(UVec2 pos, Vec2 shift) const
{
	assert(std::abs(shift.first) < size.first);
	assert(std::abs(shift.second) < size.second);
	return
	{
		((int)pos.first + shift.first + size.first) % size.first,
		((int)pos.second + shift.second + size.second) % size.second,
	};
}

char Field2::GetCell(UVec2 pos) const
{
	assert(pos.first < size.first);
	assert(pos.second < size.second);
	return board[pos.first][pos.second];
}

std::optional<UVec2> Field2::TryGetCenterPos() const
{
	if (size.first == 0 || size.second == 0)
		return std::nullopt;

	return UVec2
	{
		size.first / 2,
		size.second / 2
	};
}
