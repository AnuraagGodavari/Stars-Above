#include "starsabove_map.h"

#include "simple_logger.h"

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

	map_init(sj_array_get_count(map_json));

	for (i = 0; i < game_map.num_systems; i++)
	{
		curr_system = system_fromJson(&game_map.systems[i], sj_array_get_nth(map_json, i));

		curr_system->_inuse = 1;
		curr_system->ent->_clickable = 1;

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

	if (num_systems <= 0)
	{
		slog("Cannot initialize map with <= 1 systems"); return NULL;
	}

	game_map.num_systems = num_systems;
	game_map.systems = malloc(sizeof(System) * num_systems);
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
		if (game_map.systems[i]._inuse)
		{
			system_free(&game_map.systems[i]);
		}
	}

	memset(&game_map, 0, sizeof(Map));
}