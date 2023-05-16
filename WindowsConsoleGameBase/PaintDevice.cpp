#include "PaintDevice.h"
#include <iostream>
#include "Square.h"

namespace {
	void set_font() {
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof cfi;
		cfi.nFont = 0;
		cfi.dwFontSize.X = 18;
		cfi.dwFontSize.Y = 18;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"Lucida Console");
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}
}

PaintDevice::PaintDevice() 
	: m_Size(40,40) {
	set_font();
	m_ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);

	resize(m_Size);

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(m_ConsoleOut, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(m_ConsoleOut, &cursorInfo);
}

PaintDevice::~PaintDevice() {
	delete m_Buffer;
}

bool PaintDevice::ready() const {
	return m_Ready;
}

void PaintDevice::resize(const Size& size) {
	m_Size = size;
	SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(m_Size.width() - 1), static_cast<SHORT>(m_Size.height() - 1) };
	COORD windowBufSize = { static_cast<SHORT>(m_Size.width()), static_cast<SHORT>(m_Size.height()) };

	m_Ready = false;

	SMALL_RECT screen = { 0, 0, 1, 1 };
	if (!SetConsoleWindowInfo(m_ConsoleOut, TRUE, &screen)) {
		std::cout << __LINE__ << ": MAX SetConsoleScreenBufferSize failed with error " << GetLastError() << std::endl;
		return;
	}

	if (!SetConsoleScreenBufferSize(m_ConsoleOut, windowBufSize)) {
		std::cout << __LINE__ << ": SetConsoleScreenBufferSize failed with error " << GetLastError() << std::endl;
		return;
	}

	if (!SetConsoleWindowInfo(m_ConsoleOut, TRUE, &windowSize)) {
		std::cout << __LINE__ << ": SetConsoleWindowInfo failed with error " << GetLastError() << std::endl;
		return;
	}

	m_Ready = true;

	if (m_Buffer) delete[] m_Buffer;

	m_Buffer = new CHAR_INFO[m_Size.area()];
	m_BufferSize.X = m_Size.width();
	m_BufferSize.Y = m_Size.height();
	m_BufferCoord.X = 0;
	m_BufferCoord.Y = 0;

	m_WriteRegion.Left = 0;
	m_WriteRegion.Top = 0;
	m_WriteRegion.Right = m_Size.width();
	m_WriteRegion.Bottom = m_Size.height();

	clear();
}

void PaintDevice::clear() {
	if (!m_Ready) return;
	for (int x = 0; x < m_BufferSize.X; x++)
	{
		for (int y = 0; y < m_BufferSize.Y; y++)
		{
			set_char(Vector2(x, y), L' ');
		}
	}
}

void PaintDevice::set_char(const Vector2& position, wchar_t c, WORD attr) {
	if (!m_Ready) return;
	if (Square(Vector2(0, 0), m_Size).hit(position)) {
		const int idx = position.x() + m_BufferSize.X * position.y();
		m_Buffer[idx].Char.UnicodeChar = c;
		m_Buffer[idx].Attributes = attr;
	}
}

void PaintDevice::set_char(const Vector2& position, CHAR_INFO char_info) {
	if (!m_Ready) return;
	if (Square(Vector2(0, 0), m_Size).hit(position)) {
		const int idx = position.x() + m_BufferSize.X * position.y();
		m_Buffer[idx].Char.UnicodeChar = char_info.Char.UnicodeChar;
		m_Buffer[idx].Attributes = char_info.Attributes;
	}
}

wchar_t PaintDevice::get_char(const Vector2& position) {
	if (!m_Ready) return L'\0';
	if (Square(Vector2(0, 0), m_Size).hit(position)) {
		const int idx = position.x() + m_BufferSize.X * position.y();
		return m_Buffer[idx].Char.UnicodeChar;
	}
	return L'\0';
}

void PaintDevice::render() {
	if (!m_Ready) return;
	if (!WriteConsoleOutput(m_ConsoleOut, m_Buffer, m_BufferSize, m_BufferCoord, &m_WriteRegion))
		std::cout << __LINE__ << ": WriteConsoleOutput failed - (%d)\n" << GetLastError() << std::endl;
}
