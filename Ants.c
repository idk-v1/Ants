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
	uint32_t dir;
} Ant;

typedef struct Colony
{
	vec2f pt;
	Ant* ants;
	uint64_t numAnts;
	uint64_t maxAnts;
} Colony;

static Colony createColony(sft_window* win)
{
	Colony colony;
	colony.pt.x = win->width / 2;
	colony.pt.y = win->height / 2;

	colony.numAnts = 0;
	colony.maxAnts = 1;
	colony.ants = malloc(colony.maxAnts * sizeof(Ant));
	return colony;
}

static void addAnt(Colony* colony)
{
	// if array is filled, resize
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

	// spawn ant at the colony (middle of screen)
	// ant has random direction
	for (uint64_t i = 0; i < ANT_SEGS; i++)
		colony->ants[colony->numAnts].pts[i] = colony->pt;
	colony->ants[colony->numAnts].dir = rand() % (360 / 5) * 5;

	colony->numAnts++;
}

static void updateAnts(sft_window* win, Colony* colony)
{
	for (uint64_t i = 0; i < colony->numAnts; i++)
	{
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
	}
}

static void drawAnts(sft_window* win, Colony* colony)
{
	for (uint64_t i = 0; i < colony->numAnts; i++)
		for (uint64_t s = 0; s < ANT_SEGS; s++)
			sft_window_drawRect(win, colony->ants[i].pts[s].x, 
				colony->ants[i].pts[s].y, 1, 1, 0xFF000000);
}

int main()
{
	srand(time(NULL));

	sft_init();

	sft_window* win = sft_window_open("", 0, 0, 0, 0,
		sft_flag_fullscreen | sft_flag_passthru | sft_flag_syshide | sft_flag_topmost);

	Colony colony = createColony(win);
	addAnt(&colony);

	while (sft_window_update(win))
	{
		// Randomly add an ant
		if (rand() % 100 == 0)
			addAnt(&colony);
		
		updateAnts(win, &colony);

		sft_window_fill(win, 0x00000000);
		drawAnts(win, &colony);
		sft_window_display(win);

		sft_sleep(10);
	}
	sft_window_close(win);

	sft_shutdown();
	return 0;
}