#include <vector>
#include <optional>
#include "Vec2.h"

class Field2
{
public:
	Field2(unsigned char width, unsigned char heigth, char defaultCellFiller);

	void Clear();
	std::optional<UVec2> TryFindFreeRandomPos() const;
	void SetCell(UVec2 pos, char symbol);
	size_t GetWidth() const { return size.second; };
	size_t GetHeight() const { return board.size(); };
	size_t GetFreePosCount() const { return freePositions.size(); };
	const std::vector<std::vector<char>>& GetLines() const { return board; };
	UVec2 GetShiftedPosition(UVec2 pos, Vec2 shift) const;
	char GetCell(UVec2 pos) const;
	std::optional<UVec2> TryGetCenterPos() const;

	inline static const Vec2 DIR_UP{ -1, 0 };
	inline static const Vec2 DIR_LEFT{ 0, -1 };
	inline static const Vec2 DIR_DOWN{ 1, 0 };
	inline static const Vec2 DIR_RIGHT{ 0, 1 };

private:
	std::vector<std::vector<char>> board;
	std::vector<UVec2> freePositions;
	const UVec2 size;
	const char cellFiller;
};
