#include "gf2d_draw.h"
#include "simple_logger.h"

#include "starsabove_map.h"

#include "camera.h"

Map game_map = { 0 };

Map* map_fromJson(SJson* map_json)
{
	int i;

	System* curr_system;

	if (game_map.systems)
	{
		map_free();
	}

	if (!map_json)
	{
		slog("Cannot load map from NULL json"); return;
	}

	if (!sj_is_array(map_json))
	{
		slog("Cannot load map from non-array json"); return;
	}

	//Initialize the map with the number of systems
	map_init(sj_array_get_count(map_json));

	for (i = 0; i < game_map.num_systems; i++)
	{
		curr_system = system_fromJson(&game_map.systems[i], sj_array_get_nth(map_json, i));

		//TEMPORARY: Remove when adding GameEvents
		game_map.systems[i].ent->_clickable = 1;

	}

	for (i = 0; i < game_map.num_systems; i++)
	{
		curr_system = &game_map.systems[i];

		system_add_all_neighbors(curr_system, sj_array_get_nth(map_json, i));

	}

}

void map_init(Uint32 num_systems)
{
	int i;

	if (num_systems <= 1)
	{
		slog("Cannot initialize map with <= 1 systems"); return NULL;
	}

	game_map.num_systems = num_systems;
	game_map.systems = malloc(sizeof(System) * num_systems);
}

void map_drawPaths()
{
	int i; int j;

	if (!game_map.systems)
	{
		slog("Cannot draw paths for unitialized map"); return;
	}

	for (i = 0; i < game_map.num_systems; i++)
	{

		for (j = 0; j < game_map.systems[i].num_neighbor_systems; j++)
		{
			//NOTE: Maybe add and compare IDs? 'a' value in vector4d must be half of what we actually want because of twice-over redraws
			gf2d_draw_line
			(
				game_map.systems[i].ent->collidercircle->viewpos,
				game_map.systems[i].neighbor_systems[j]->ent->collidercircle->viewpos,
				vector4d(255, 255, 255, 50)
			);
		}
	}

}

void map_free()
{
	int i;

	if (!game_map.systems)
	{
		slog("Cannot free NULL map"); return;
	}

	for (i = 0; i < game_map.num_systems; i++)
	{
		system_free(&game_map.systems[i]);
	}

	memset(&game_map, 0, sizeof(Map));
}

Map* get_map()
{
	return &game_map;
}