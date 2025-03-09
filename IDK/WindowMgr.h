#pragma once
#include "Window.h"

typedef struct IDKIntWindow
{
	void* handle;
	uint64 width;
	uint64 height;
	int64 left;
	int64 top;
	uint64 style;
	string title;
} IDKIntWindow;

typedef struct IDKWindowMgr
{
	IDKIntWindow** windows;
	uint64 numWindows;
	uint64 maxWindows;
} IDKWindowMgr;

extern bool idkWindowInit;
extern IDKWindowMgr idkWindowMgr;


bool idkInitWindow();
void idkExitWindow();

IDKIntWindow* idkWindowMgr_getPtr(IDKWindow window);

IDKWindow idkWindowMgr_add();
bool idkWindowMgr_remove(IDKWindow window);

void idkWindowMgr_update();