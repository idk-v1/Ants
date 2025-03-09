#pragma once
#include "Util.h"

enum idkKeys
{
	IDK_KEY_NULL	= 0x00,
	IDK_KEY_ESC		= 0x1B,

	IDK_KEY_COUNT
};

bool idkKey(uint64 key);