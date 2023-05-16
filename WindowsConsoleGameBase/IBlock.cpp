#include "IBlock.h"

IBlock::IBlock(Vector2 position) : Figure(position)
{
	m_Bodies = { 
		{ Vector2(1,0), Vector2(1,1), Vector2(1,2), Vector2(1,3) },
		{ Vector2(0,2), Vector2(1,2), Vector2(2,2), Vector2(3,2) },
		{ Vector2(2,0), Vector2(2,1), Vector2(2,2), Vector2(2,3) },
		{ Vector2(0,1), Vector2(1,1), Vector2(2,1), Vector2(3,1) } 
	};
}
