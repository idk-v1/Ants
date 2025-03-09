#include "WindowMgr.h"
#include "Window.h"
#include "Util.h"

IDKWindow idkWindow_create(cstring title, uint64 width, uint64 height, uint64 flags)
{
	IDKWindow window = idkWindowMgr_add();
	IDKIntWindow* winPtr = idkWindowMgr_getPtr(window);

	return window;
}

void idkWindow_update()
{
	idkWindowMgr_update();
}

bool idkWindow_isOpen(IDKWindow window)
{
	IDKIntWindow* winPtr = idkWindowMgr_getPtr(window);
	return winPtr != NULL;
}

IDKSize idkWindow_getSize(IDKWindow window)
{
	IDKSize size = { 0, 0 };
	IDKIntWindow* winPtr = idkWindowMgr_getPtr(window);
	if (winPtr)
	{
		size.w = winPtr->width;
		size.h = winPtr->height;
	}

	return size;
}

void idkWindow_setSize(IDKWindow window, uint64 width, uint64 height)
{
}

IDKPos idkWindow_getPos(IDKWindow window)
{
	IDKPos pos = { 0, 0 };
	IDKIntWindow* winPtr = idkWindowMgr_getPtr(window);
	if (winPtr)
	{
		pos.x = winPtr->left;
		pos.y = winPtr->top;
	}

	return pos;
}

void idkWindow_setPos(IDKWindow window, int64 x, uint64 y)
{
}

cstring idkWindow_getTitle(IDKWindow window)
{
	IDKIntWindow* winPtr = idkWindowMgr_getPtr(window);
	if (winPtr)
		return winPtr->title;

	return NULL;
}

void idkWindow_setTitle(IDKWindow window, cstring title)
{
}

bool idkWindow_getFlag(IDKWindow window, uint64 flag)
{
	IDKIntWindow* winPtr = idkWindowMgr_getPtr(window);
	if (winPtr)
		return winPtr->style & flag;

	return false;
}

void idkWindow_setFlag(IDKWindow window, uint64 flag, bool state)
{
}

void idkWindow_close(IDKWindow window)
{
	idkWindowMgr_remove(window);
}

IDKSize idkWindow_getScreenSize()
{
	IDKSize size = { 0, 0 };
	// TODO everything
	return size;
}
