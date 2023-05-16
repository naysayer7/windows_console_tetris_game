#include "GameField.h"
#include "Colors.h"

void GameField::resize(Size size)
{
	if (m_FieldBuffer != nullptr)
		delete[] m_FieldBuffer;

	m_Width = size.width();
	m_Height = size.height();

	// Initializing field buffer
	m_BufferSize = Size(m_Width - 2, m_Height - 2);
	m_FieldBuffer = new CHAR_INFO[m_BufferSize.area()];
	for (size_t i = 0; i < m_BufferSize.area(); i++)
	{
		m_FieldBuffer[i].Char.UnicodeChar = EMPTY_FIELD_SYMBOL;
		// Default color(white)
		m_FieldBuffer[i].Attributes = WHITE_COLOR;
	}
}

void GameField::render(PaintDevice& paintDevice) const
{
	// Draw horizontal borders
	for (size_t i = 1; i < m_Width - 1; i++)
	{
		// Draw top border
		Vector2 top_v(i, 0);
		paintDevice.set_char(top_v, HORIZONTAL_BORDER_SYMBOL, BORDER_COLOR);
		// Draw bottom border
		Vector2 bottom_v(i, m_Height - 1);
		paintDevice.set_char(bottom_v, HORIZONTAL_BORDER_SYMBOL, BORDER_COLOR);
	}

	// Draw vertical borders
	for (size_t i = 1; i < m_Height - 1; i++)
	{
		// Draw right border
		Vector2 right_v(m_Width - 1, i);
		paintDevice.set_char(right_v, VERTICAL_BORDER_SYMBOL, BORDER_COLOR);
		// Draw left border
		Vector2 left_v(0, i);
		paintDevice.set_char(left_v, VERTICAL_BORDER_SYMBOL, BORDER_COLOR);
	}

	// Draw top left corner
	Vector2 tl_v(0, 0);
	paintDevice.set_char(tl_v, TL_CORNER_SYMBOL, BORDER_COLOR);

	// Draw top right corner
	Vector2 tr_v(m_Width - 1, 0);
	paintDevice.set_char(tr_v, TR_CORNER_SYMBOL, BORDER_COLOR);

	// Draw bottom left corner
	Vector2 bl_v(0, m_Height - 1);
	paintDevice.set_char(bl_v, BL_CORNER_SYMBOL, BORDER_COLOR);

	// Draw bottom right corner
	Vector2 br_v(m_Width - 1, m_Height - 1);
	paintDevice.set_char(br_v, BR_CORNER_SYMBOL, BORDER_COLOR);

	// Draw field content
	for(size_t y = 0; y < m_Height - 2; y++)
		for (size_t x = 0; x < m_Width - 2; x++)
		{
			Vector2 v(x + 1, y + 1);
			paintDevice.set_char(v, get_char_info(Vector2(x, y)));
		}

}

bool GameField::has_collision(const Figure& figure) const
{
	Vector2 position = figure.get_position();
	for (const Vector2& point : figure.get_body())
	{
		// Vertical border check
		if (point.x() + position.x() < 1 || point.x() + position.x() > m_Width - 2)
			return true;
		// Horizontal border check
		if (point.y() + position.y() < 1 || point.y() + position.y() > m_Height - 2)
			return true;
		// Blocks collision check
		if(get_char(point + position - Vector2(1, 1)) != EMPTY_FIELD_SYMBOL)
			return true;
	}

	return false;
}

unsigned int GameField::merge(const Figure& figure)
{
	// Place the figure
	Vector2 position = figure.get_position();
	for (const Vector2& point : figure.get_body())
		set_char(point + position - Vector2(1, 1), FIGURE_SYMBOL, figure.get_attr());

	unsigned int counter = 0;

	// Check if there's line to delete
	for (size_t y = 0; y < m_Height - 2; y++) 
	{
		// 'full' line must contain only chars != EMPTY_FIELD_SYMBOL
		bool is_full = true;
		for (size_t x = 0; x < m_Width - 2; x++)
			is_full = is_full && get_char(Vector2(x, y)) != EMPTY_FIELD_SYMBOL;

		if (is_full)
		{
			// Offset the lines above full line
			for (size_t i = y; i > 0; i--)
				for (size_t j = 0; j < m_BufferSize.width(); j++)
					set_char(Vector2(j, i), get_char_info(Vector2(j, i - 1)));
			// Clear the first line
			for (size_t i = 0; i < m_BufferSize.width(); i++)
				set_char(Vector2(i, 0), EMPTY_FIELD_SYMBOL);
			++counter;
		}
	}

	return counter;
}

wchar_t GameField::get_char(Vector2 position) const
{
	// Range check
	if (Square(Vector2(0, 0), m_BufferSize).hit(position))
	{
		const int idx = position.x() + m_BufferSize.width() * position.y();
		return m_FieldBuffer[idx].Char.UnicodeChar;
	}
	return L'\0';
}

CHAR_INFO GameField::get_char_info(const Vector2& position) const
{
	// Range check
	if (Square(Vector2(0, 0), m_BufferSize).hit(position))
	{
		const int idx = position.x() + m_BufferSize.width() * position.y();
		return m_FieldBuffer[idx];
	}
	return CHAR_INFO();
}

void GameField::set_char(Vector2 position, wchar_t c, WORD attributes)
{
	// Range check
	if (Square(Vector2(0, 0), m_BufferSize).hit(position))
	{
		const int idx = position.x() + m_BufferSize.width() * position.y();
		m_FieldBuffer[idx].Char.UnicodeChar = c;
		m_FieldBuffer[idx].Attributes = attributes;
	}
}

void GameField::set_char(Vector2 position, CHAR_INFO char_info)
{
	// Range check
	if (Square(Vector2(0, 0), m_BufferSize).hit(position))
	{
		const int idx = position.x() + m_BufferSize.width() * position.y();
		m_FieldBuffer[idx].Char.UnicodeChar = char_info.Char.UnicodeChar;
		m_FieldBuffer[idx].Attributes = char_info.Attributes;
	}
}

GameField::~GameField()
{
	delete[] m_FieldBuffer;
}
