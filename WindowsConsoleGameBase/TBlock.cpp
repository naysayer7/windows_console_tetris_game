#include "TBlock.h"

TBlock::TBlock(Vector2 position) : Figure(position)
{
	m_Bodies = {
		{Vector2(1, 0), Vector2(0, 1), Vector2(1, 1), Vector2(2, 1)},
		{Vector2(1, 0), Vector2(1, 1), Vector2(2, 1), Vector2(1, 2)},
		{Vector2(0, 1), Vector2(1, 1), Vector2(2, 1), Vector2(1, 2)},
		{Vector2(0, 1), Vector2(1, 1), Vector2(1, 0), Vector2(1, 2)}
	};
}
