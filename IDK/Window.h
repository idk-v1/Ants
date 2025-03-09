#pragma once
#include "Util.h"

typedef uint64 IDKWindow;

enum idkWindowFlags
{
	IDK_FLAG_DEFAULT		= 0,
	IDK_FLAG_FULLSCREEN		= 1 << 0x00,
	IDK_FLAG_TRANSPARENT	= 1 << 0x01,
	IDK_FLAG_PASSTHRU		= 1 << 0x02,
	IDK_FLAG_BORDERLESS		= 1 << 0x03,
	IDK_FLAG_FIXEDSIZE		= 1 << 0x04,
	IDK_FLAG_USERTHEME		= 1 << 0x05,
	IDK_FLAG_MAXIMIZED		= 1 << 0x06,
	IDK_FLAG_MINIMIZED		= 1 << 0x07,
};

IDKWindow idkWindow_create(cstring title, uint64 width, uint64 height, uint64 flags);

void idkWindow_update();

bool idkWindow_isOpen(IDKWindow window);

IDKSize idkWindow_getSize(IDKWindow window);
void idkWindow_setSize(IDKWindow window, uint64 width, uint64 height);

IDKPos idkWindow_getPos(IDKWindow window);
void idkWindow_setPos(IDKWindow window, int64 x, uint64 y);

cstring idkWindow_getTitle(IDKWindow window);
void idkWindow_setTitle(IDKWindow window, cstring title);

bool idkWindow_getFlag(IDKWindow window, uint64 flag);
void idkWindow_setFlag(IDKWindow window, uint64 flag, bool state);

void idkWindow_close(IDKWindow window);

IDKSize idkWindow_getScreenSize();