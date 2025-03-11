#include "softdraw/softdraw.h"

#include <math.h>
#include <time.h>

#define TO_RAD(deg) (deg / 180.f * 3.1415f)

#define ANT_SEGS 3

typedef struct vec2f
{
	float x, y;
} vec2f;

typedef struct Ant
{
	vec2f pts[ANT_SEGS];
	int32_t dir;
	bool deleted;
} Ant;

typedef struct Colony
{
	vec2f pt;
	Ant* ants;
	uint64_t count;
	uint64_t numAnts;
	uint64_t maxAnts;
} Colony;

static bool pointInRect(float x, float y, float left, float top, float width, float height)
{
	return (x >= left && y >= top && x < width && y < height);
}

static bool pointInCircle(float x, float y, float cx, float cy, float radius)
{
	return (hypotf(x - cx, y - cy) < radius);
}

static void setPixel(sft_image* image, int64_t x, int64_t y, sft_color color)
{
	if (x < 0 || y < 0 || x >= image->width || y >= image->height)
		return;

	image->pixels[x + y * image->width] = color;
}

static sft_color getPixel(sft_image* image, int64_t x, int64_t y)
{
	if (x < 0 || y < 0 || x >= image->width || y >= image->height)
		return 0x00000000;

	return image->pixels[x + y * image->width];
}

static Colony createColony(sft_window* win)
{
	Colony colony;
	colony.pt.x = win->width / 2;
	colony.pt.y = win->height / 2;

	colony.count = 0;
	colony.numAnts = 0;
	colony.maxAnts = 1;
	colony.ants = malloc(colony.maxAnts * sizeof(Ant));
	return colony;
}

static void deleteColony(Colony* colony)
{
	free(colony->ants);
	colony->ants = NULL;
	colony->count = 0;
	colony->numAnts = 0;
	colony->maxAnts = 0;
}

static void deleteAnt(Colony* colony, uint64_t index)
{
	if (index < colony->numAnts)
	{
		colony->ants[index].deleted = true;
		colony->count--;
	}
}

static void addAnt(Colony* colony)
{
	// search for deleted ant in array to replace first
	bool replace = false;

	uint64_t i = 0;
	for (; i < colony->numAnts; i++)
		if (colony->ants[i].deleted)
		{
			replace = true;
			break;
		}

	// if need to add new ant instead of replacing
	// if array is filled, resize
	if (!replace)
	{
		if (colony->numAnts + 1 >= colony->maxAnts)
		{
			Ant* temp = realloc(colony->ants, colony->maxAnts * 2 * sizeof(Ant));
			if (temp)
			{
				colony->maxAnts *= 2;
				colony->ants = temp;
			}
			else
				return; // no more memory, keep existing ants
		}

		i = colony->numAnts;
		colony->numAnts++;
	}

	// spawn ant at the colony (middle of screen)
	// ant has random direction
	for (uint64_t s = 0; s < ANT_SEGS; s++)
		colony->ants[i].pts[s] = colony->pt;
	colony->ants[i].dir = rand() % (360 / 5) * 5;
	colony->ants[i].deleted = false;
	colony->count++;
}

static void updateAnts(sft_window* win, Colony* colony, sft_image* background)
{
	sft_point mouse = sft_input_mousePos(win);

	for (uint64_t i = 0; i < colony->numAnts; i++)
	{
		if (colony->ants[i].deleted)
			continue;

		// move the last segment to the next segment,
		// then the next segment to the next next segment, repeat
		// the head is controlled by the direction
		// the other body segments follow the head, 
		// justs delayed by an update
		for (uint64_t s = ANT_SEGS - 1; s > 0; s--)
			colony->ants[i].pts[s] = colony->ants[i].pts[s - 1];

		colony->ants[i].pts[0].x += cosf(TO_RAD(colony->ants[i].dir));
		colony->ants[i].pts[0].y += sinf(TO_RAD(colony->ants[i].dir));

		// chance to adjust direction
		if (rand() % 10 == 0)
			colony->ants[i].dir += (rand() % 2 ? -5 : 5);

		// wrap direction between 0 and 360
		if (colony->ants[i].dir < 0)
			colony->ants[i].dir += 360;
		if (colony->ants[i].dir >= 360)
			colony->ants[i].dir -= 360;

		float antX = round(colony->ants[i].pts[0].x);
		float antY = round(colony->ants[i].pts[0].y);

		// Ant made it to border
		// or head is touching stolen pixel
		if (!pointInRect(antX, antY, 0, 0, background->width, background->height) ||
			// small chance to ignore pixel and take a buried one
			(getPixel(background, antX, antY) && rand() % 10))
		{
			// set last position to stolen (2nd segment)
			setPixel(background, antX, antY, 0xFF000000);

			// reset ant and add another
			deleteAnt(colony, i);
			addAnt(colony);
			addAnt(colony);
		}

		// If clicking near ant, squish it
		if (sft_input_clickState(sft_click_Left) && 
			pointInCircle(antX, antY, mouse.x, mouse.y, 5.f))
		{
			deleteAnt(colony, i);
		}
	}
}

static void drawAnts(sft_window* win, Colony* colony)
{
	static int flash = 0;
	flash++;

	// draw colony
	sft_window_drawRect(win, colony->pt.x - 2, colony->pt.y - 2, 5, 5, 0x7F7F3F00);

	// debug (or just bug) thingy to make ants easier to see
	for (uint64_t i = 0; i < colony->numAnts; i++)
	{
		if (colony->ants[i].deleted)
			continue;

#ifdef _DEBUG
		if (sft_input_keyState(sft_key_Capslock) && flash / 10 % 2)
		{
			for (uint64_t s = 0; s < ANT_SEGS; s++)
				sft_window_drawRect(win, colony->ants[i].pts[s].x, colony->ants[i].pts[s].y, 1, 1, 0xFFFF0000);
		}
		else
		{
			for (uint64_t s = 0; s < ANT_SEGS; s++)
				sft_window_drawRect(win, colony->ants[i].pts[s].x, colony->ants[i].pts[s].y, 1, 1, 0xFF000000);
		}
#else
		for (uint64_t s = 0; s < ANT_SEGS; s++)
			sft_window_drawRect(win, colony->ants[i].pts[s].x, colony->ants[i].pts[s].y, 1, 1, 0xFF000000);
#endif
	}
}

int main()
{
	srand(time(NULL));

	sft_init();

	sft_window* win = sft_window_open("", 0, 0, 0, 0,
		sft_flag_fullscreen | sft_flag_passthru | 
		sft_flag_syshide | sft_flag_topmost);
	sft_window_fill(win, 0x00000000);
	sft_window_display(win);


	sft_image* background = sft_image_create(win->width, win->height);
	sft_image_fill(background, 0x00000000);

	Colony colony = createColony(win);
	addAnt(&colony);

	while (sft_window_update(win))
	{
		sft_input_update();

#ifdef _DEBUG
		if (sft_input_keyPressed(sft_key_Escape))
			break;
#endif

		// Randomly add an ant
		if (rand() % 100 == 0)
			addAnt(&colony);
		
		updateAnts(win, &colony, background);

		// instead of clearing background, draw stolen pixels 
		// bc i didn't add alpha blending, so it works
		sft_window_drawImage(win, background, 0, 0, 
			background->width, background->height, 0, 0);

		drawAnts(win, &colony);

		// debug bug counter
#ifdef _DEBUG
		if (sft_input_keyState(sft_key_Capslock))
		{
			sft_window_drawTextF(win, 0, 0, 4, 0xFFFF00FF, "%9llu", colony.count);

			// debug squish radius display (actually a circle, just drawn as square)
			sft_window_drawRect(win, sft_input_mousePos(win).x - 5, sft_input_mousePos(win).y - 5, 10, 10, 0xFFFF0000);
		}
#endif

		sft_window_display(win);

		sft_sleep(10);
	}

	sft_image_delete(background);
	deleteColony(&colony);
	sft_window_close(win);

	sft_shutdown();
	return 0;
}