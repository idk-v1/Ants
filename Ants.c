#include "IDK/Window.h"
#include "IDK/Keyboard.h"

int main()
{
	IDKWindow window = idkWindow_create("Ants", 0, 0,
		IDK_FLAG_FULLSCREEN | IDK_FLAG_TRANSPARENT | IDK_FLAG_PASSTHRU);

	bool running = window;
	while (running)
	{
		idkWindow_update();

		if (idkKey(IDK_KEY_ESC))
		{
			idkWindow_close(window);
			running = false;
		}
	}

	return 0;
}