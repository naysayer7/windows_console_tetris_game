#include "BlockFactory.h"

Figure* BlockFactory::get_figure(const size_t name, const Vector2 position)
{
	if (name == 0)
		return new IBlock(position);
	if (name == 1)
		return new JBlock(position);
	if (name == 2)
		return new LBlock(position);
	if (name == 3)
		return new OBlock(position);
	if (name == 4)
		return new SBlock(position);
	if (name == 5)
		return new TBlock(position);
	if (name == 6)
		return new ZBlock(position);
	return nullptr;
}
