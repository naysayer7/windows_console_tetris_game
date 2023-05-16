#include "Figure.h"
#include "Colors.h"
#include <random>

Figure::Figure(Vector2 position) : m_Position(position) 
{
	backup();

	switch (rand() % 7)
	{
	case 0:
		m_Attributes = RED_COLOR;
		break;
	case 1:
		m_Attributes = GREEN_COLOR;
		break;
	case 2:
		m_Attributes = BLUE_COLOR;
		break;
	case 3:
		m_Attributes = YELLOW_COLOR;
		break;
	case 4:
		m_Attributes = PURPLE_COLOR;
		break;
	case 5:
		m_Attributes = LIGHT_BLUE_COLOR;
		break;
	case 6:
		m_Attributes = WHITE_COLOR;
		break;
	default:
		break;
	}
}

void Figure::update(const int dt)
{
	m_TimeFromLastUpdate += dt;
	if (m_TimeFromLastUpdate > m_TimeForUpdate)
	{
		m_TimeFromLastUpdate = 0;
		++m_Position.y();
	}
}

void Figure::render(PaintDevice& paintDevice) const
{
	for (const Vector2& point : m_Bodies[m_State])
	{
		Vector2 v(point.x() + m_Position.x(),
			point.y() + m_Position.y());
		paintDevice.set_char(v, FIGURE_SYMBOL, m_Attributes);
	}
}


void Figure::move_right()
{
	++m_Position.x();
}

void Figure::move_left()
{
	--m_Position.x();
}

void Figure::move_down()
{
	++m_Position.y();
}

void Figure::backup()
{
	m_PositionBackup = m_Position;
	m_StateBackup = m_State;
}

void Figure::restore()
{
	m_Position = m_PositionBackup;
	m_State = m_StateBackup;
}

void Figure::rotate()
{
	if (m_State + 1 < m_Bodies.size())
		++m_State;
	else
		m_State = 0;
}
