#include "softdraw/softdraw.h"

#include <math.h>
#include <time.h>

#define TO_RAD(deg) (deg / 180.f * 3.1415f)
#define TO_DEG(rad) (rad / 3.1415f * 180.f)

#define ANT_SEGS 3
#define SQUISH_RANGE 5

typedef struct vec2f
{
	float x, y;
} vec2f;

typedef struct Ant
{
	vec2f pts[ANT_SEGS];
	vec2f target;
	float dir;
	bool deleted;
	bool hasTarget;
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
	return (x >= left && y >= top && x < left + width && y < top + height);
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
	colony->ants[i].hasTarget = false;
	colony->count++;
}

static void updateAnts(sft_window* win, Colony* colony, sft_image* background)
{
	sft_point mouse = sft_input_mousePos(win);

	for (uint64_t i = 0; i < colony->numAnts; i++)
	{
		Ant* ant = &colony->ants[i];
		if (ant->deleted)
			continue;

				// if clicking near ant, squish it
		if (sft_input_clickState(sft_click_Left) &&
			pointInRect(ant->pts[0].x, ant->pts[0].y, mouse.x - SQUISH_RANGE,
				mouse.y - SQUISH_RANGE, SQUISH_RANGE * 2, SQUISH_RANGE * 2))
		{
			deleteAnt(colony, i);
			continue;
		}

		// check if ant has a target or another ant took the target
		if (!ant->hasTarget ||
			getPixel(background, ant->target.x, ant->target.y) == 0xFF000000)
		{
			ant->hasTarget = false;
			ant->target.x = rand() % background->width;
			ant->target.y = rand() % background->height;

			// check if target is taken
			if (getPixel(background, ant->target.x, ant->target.y) != 0xFF000000)
			{
				// surrounding pixel was taken already
				if (getPixel(background, ant->target.x + 1, ant->target.y) == 0xFF000000 ||
					getPixel(background, ant->target.x, ant->target.y + 1) == 0xFF000000 ||
					getPixel(background, ant->target.x - 1, ant->target.y) == 0xFF000000 ||
					getPixel(background, ant->target.x, ant->target.y - 1) == 0xFF000000 ||
					// target is on edge
					ant->target.x == 0 ||
					ant->target.y == 0 ||
					ant->target.x == background->width - 1 ||
					ant->target.y == background->height - 1)
				{
					ant->hasTarget = true;
					ant->dir = TO_DEG(atan2f(
						ant->target.x - ant->pts[0].x,
						ant->target.y - ant->pts[0].y));
				}
			}
		}
		if (!ant->hasTarget)
			continue; // chose one next time, otherwise infinite loop possible

		// move the last segment to the next segment,
		// then the next segment to the next next segment, repeat
		// the head is controlled by the direction
		// the other body segments follow the head, 
		// justs delayed by an update
		for (uint64_t s = ANT_SEGS - 1; s > 0; s--)
			ant->pts[s] = ant->pts[s - 1];

		ant->pts[0].x += sinf(TO_RAD(ant->dir));
		ant->pts[0].y += cosf(TO_RAD(ant->dir));

		// check if ant made it to target
		if (pointInCircle(ant->pts[0].x, ant->pts[0].y, ant->target.x, ant->target.y, 1.f))
		{
			// set position to stolen
			setPixel(background, ant->target.x, ant->target.y, 0xFF000000);

			// reset ant and add another
			deleteAnt(colony, i);
			addAnt(colony);
			addAnt(colony);
		}
	}
}

static void drawAnts(sft_window* win, Colony* colony)
{
	static int count = 0;
#ifdef _DEBUG
	count++;
#endif

	uint32_t color = (sft_input_keyState(sft_key_Capslock) && count / 10 % 2)
		? 0xFFFF0000 : 0xFF000000;

	// draw colony
	sft_window_drawRect(win, colony->pt.x - 2, colony->pt.y - 2, 5, 5, 0x7F7F3F00);

	for (uint64_t i = 0; i < colony->numAnts; i++)
	{
		Ant* ant = &colony->ants[i];

		if (ant->deleted)
			continue;

		for (uint64_t s = 0; s < ANT_SEGS; s++)
			sft_window_drawRect(win, ant->pts[s].x, ant->pts[s].y, 1, 1, color);
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

		// debug mode only: exit
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

			// debug squish area display
			sft_window_drawRect(win, sft_input_mousePos(win).x - SQUISH_RANGE, 
				sft_input_mousePos(win).y - SQUISH_RANGE, 
				SQUISH_RANGE * 2, SQUISH_RANGE * 2, 0xFFFF0000);
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