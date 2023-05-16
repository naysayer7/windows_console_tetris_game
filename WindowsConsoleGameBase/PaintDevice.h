#pragma once

#include <Windows.h>
#include "Size.h"
#include "Vector2.h"

class PaintDevice {
public:
	PaintDevice();
	~PaintDevice();
	bool ready() const;

	void resize(const Size& size);
	void clear();

	void set_char(const Vector2& position, wchar_t c, WORD attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	void set_char(const Vector2& position, CHAR_INFO char_info);
	wchar_t get_char(const Vector2& position);

	void render();

private:
	HANDLE m_ConsoleOut;
	CHAR_INFO* m_Buffer = nullptr;
	COORD m_BufferSize;
	COORD m_BufferCoord;
	SMALL_RECT m_WriteRegion;
	Size m_Size;
	bool m_Ready = false;
};

