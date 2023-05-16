#pragma once
#include "PaintDevice.h"
#include "Figure.h"
#include "SymbolSet.h"
#include "Square.h"
#include <vector>

using std::vector;

/// <summary>
/// Draws bordered field and contains 'dead' blocks.
/// </summary>
class GameField
{
	/// <summary>
	/// Width including border
	/// </summary>
	size_t m_Width = 14;

	/// <summary>
	/// Height including border
	/// </summary>
	size_t m_Height = 26;

	/// <summary>
	/// Size of field buffer
	/// </summary>
	Size m_BufferSize;

	/// <summary>
	/// Field buffer excluding borders
	/// </summary>
	CHAR_INFO* m_FieldBuffer;
public:
	inline GameField(): m_BufferSize(0, 0) {};

	/// <summary>
	/// Deletes buffer if any and allocates new one.
	/// </summary>
	void resize(Size size);
	void render(PaintDevice& paintDevice) const;
	
	/// <summary>
	/// Checks if there's collision with borders or 'dead' blocks.
	/// </summary>
	bool has_collision(const Figure& figure) const;

	/// <summary>
	/// Adds figure's block to field buffer keeping its attributes and clears full lines.
	/// </summary>
	/// <returns>
	/// Count of deleted lines
	/// </returns>
	unsigned int merge(const Figure& figure);
	
	/// <returns>A unicode char. L'\0' if position is out of range</returns>
	wchar_t get_char(Vector2 position) const;
	
	/// <returns>CHAR_INFO() if position is out of range</returns>
	CHAR_INFO get_char_info(const Vector2& position) const;
	
	/// <summary>
	/// Sets given char to a given position.
	/// Does nothing if the position is out of range.
	/// </summary>
	/// <param name="attributes">CHAR_INFO Attributes</param>
	void set_char(Vector2 position, wchar_t c, WORD attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	
	/// <summary>
	/// Sets given char to a given position.
	/// Does nothing if the position is out of range.
	/// </summary>
	void set_char(Vector2 position, CHAR_INFO char_info);

	~GameField();
};

