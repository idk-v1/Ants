#include "WindowMgr.h"
#include "Window.h"
#include "Util.h"

#include <string.h>
#include <stdlib.h>

bool idkWindowInit = false;
IDKWindowMgr idkWindowMgr;

bool idkInitWindow()
{
	idkWindowMgr.numWindows = 1;
	idkWindowMgr.maxWindows = 2;
	idkWindowMgr.windows = malloc(sizeof(void*) * idkWindowMgr.maxWindows);
	
	if (idkWindowMgr.windows)
	{
		for (uint64 i = 0; i < idkWindowMgr.maxWindows; i++)
			idkWindowMgr.windows[i] = NULL;
		idkWindowInit = true;
	}
	return idkWindowInit;
}

void idkExitWindow()
{
	for (uint64 i = 0; i < idkWindowMgr.maxWindows; i++)
	{
		if (idkWindowMgr.windows[i])
		{
			// TODO close os window
			if (idkWindowMgr.windows[i]->title)
				free(idkWindowMgr.windows[i]->title);
			free(idkWindowMgr.windows[i]);
			idkWindowMgr.windows[i] = NULL;
		}
	}
	free(idkWindowMgr.windows);
	idkWindowMgr.windows = NULL;
	idkWindowMgr.maxWindows = 0;
	idkWindowMgr.numWindows = 0;
	idkWindowInit = false;
}

IDKIntWindow* idkWindowMgr_getPtr(IDKWindow window)
{
	if (window < idkWindowMgr.maxWindows)
		return idkWindowMgr.windows[window];
	return NULL;
}

IDKWindow idkWindowMgr_add()
{
	if (idkWindowInit == false)
		if (idkInitWindow() == false)
			return 0;

	bool hasSpace = false;
	uint64 i = 1;
	for (; i < idkWindowMgr.maxWindows; i++)
		if (idkWindowMgr.windows[i] == NULL)
		{
			hasSpace = true;
			break;
		}

	if (hasSpace)
	{
		idkWindowMgr.windows[i] = malloc(sizeof(IDKIntWindow));
		if (idkWindowMgr.windows[i])
		{
			// TODO create os window
			idkWindowMgr.numWindows++;
			return i;
		}
	}
	else
	{
		IDKIntWindow** temp = realloc(idkWindowMgr.windows,
			sizeof(void*) * (idkWindowMgr.maxWindows + 2));
		if (temp)
		{
			idkWindowMgr.maxWindows += 2;
			idkWindowMgr.windows = temp;
			idkWindowMgr.windows[i + 1] = malloc(sizeof(IDKIntWindow));
			if (idkWindowMgr.windows[i + 1])
			{
				memset(idkWindowMgr.windows[i + 1], 0, sizeof(IDKIntWindow));
				idkWindowMgr.numWindows++;
				return i + 1;
			}
		}
		// Keep existing windows, no free yet
	}

	return 0;
}

bool idkWindowMgr_remove(IDKWindow window)
{
	if (window < idkWindowMgr.maxWindows && window != 0)
		if (idkWindowMgr.windows[window])
		{
			// TODO close os window
			if (idkWindowMgr.windows[window]->title)
				free(idkWindowMgr.windows[window]->title);
			free(idkWindowMgr.windows[window]);

			idkWindowMgr.numWindows--;

			if (idkWindowMgr.numWindows <= 1)
				idkExitWindow();

			return true;
		}

	return false;
}

void idkWindowMgr_update()
{
	for (uint64 i = 1; i < idkWindowMgr.maxWindows; i++)
	{
		if (idkWindowMgr.windows[i])
		{
			// TODO update os window
		}
	}
}
