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

static void updateAnts(sft_window* win, Colony* colony)
{
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

		// Ant made it to border
		if (colony->ants[i].pts[0].x >= win->width - 1 ||
			colony->ants[i].pts[0].y >= win->height - 1 ||
			colony->ants[i].pts[0].x <= 0 ||
			colony->ants[i].pts[0].y <= 0)
		{
			// TODO steal pixel
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

		for (uint64_t s = 0; s < ANT_SEGS; s++)
		{
			if (sft_input_keyState(sft_key_Capslock))
				sft_window_drawRect(win, colony->ants[i].pts[s].x,
					colony->ants[i].pts[s].y, 5, 5, flash / 10 % 2 ? 0xFF000000 : 0xFFFF0000);
			else
				sft_window_drawRect(win, colony->ants[i].pts[s].x,
					colony->ants[i].pts[s].y, 1, 1, 0xFF000000);
		}
	}
}

int main()
{
	srand(time(NULL));

	sft_init();

	sft_window* win = sft_window_open("", 0, 0, 0, 0,
		sft_flag_fullscreen | sft_flag_passthru | 
		sft_flag_syshide | sft_flag_topmost);

	Colony colony = createColony(win);
	addAnt(&colony);

	while (sft_window_update(win))
	{
		sft_input_update();

		if (sft_input_keyPressed(sft_key_Escape))
			break;

		// Randomly add an ant
		if (rand() % 100 == 0)
			addAnt(&colony);
		
		updateAnts(win, &colony);

		sft_window_fill(win, 0x00000000);
		drawAnts(win, &colony);
		sft_window_display(win);

		sft_sleep(10);
	}

	deleteColony(&colony);
	sft_window_close(win);

	sft_shutdown();
	return 0;
}