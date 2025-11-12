#include "imgui.h"

namespace Color
{
	inline const ImColor EMPTY(0.2f, 0.2f, 0.2f, 1.0f);
	inline const ImColor SNAKE_BODY(0.0f, 0.6f, 0.2f, 1.0f);
	inline const ImColor SNAKE_HEAD(0.0f, 0.8f, 0.1f, 1.0f);
	inline const ImColor APPLE(0.8f, 0.0f, 0.0f, 1.0f);
	inline const ImColor DEFAULT(0.3f, 0.3f, 0.4f, 1.0f);
	inline const ImColor WHITE(1.0f, 1.0f, 1.0f, 1.0f);
	inline const ImColor BACKGROUND(0.1f, 0.1f, 0.15f, 1.0f);
}

namespace Cell
{
	inline const char BORDER_HORIZONTAL = '-';
	inline const char BORDER_VERTICAL = '|';
	inline const char EMPTY = ' ';
	inline const char APPLE = '@';
	inline const char SNAKE_HEAD = 's';
	inline const char SNAKE_BODY = '*';
}