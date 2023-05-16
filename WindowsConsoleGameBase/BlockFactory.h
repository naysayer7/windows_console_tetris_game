#pragma once
#include "Figure.h"
#include "IBlock.h"
#include "JBlock.h"
#include "LBlock.h"
#include "OBlock.h"
#include "SBlock.h"
#include "TBlock.h"
#include "ZBlock.h"
#include <string>

class BlockFactory
{
public:
	static Figure* get_figure(const size_t name, const Vector2 position);
};

