#include "gf2d_draw.h"
#include "simple_logger.h"

#include "starsabove_map.h"

#include "camera.h"

Map game_map = { 0 };

Map* map_fromJson(SJson* map_json)
{
	int i;

	SJson* systems_arr;
	System* curr_system;

	if (game_map.systems)
	{
		map_free();
	}

	if (!map_json)
	{
		slog("Cannot load map from NULL json"); return;
	}

	systems_arr = sj_object_get_value(map_json, "Systems");

	if (!sj_is_array(systems_arr))
	{
		slog("Cannot load systems from non-array json"); return;
	}

	//Initialize the map with the number of systems
	map_init(sj_array_get_count(systems_arr));

	for (i = 0; i < game_map.num_systems; i++)
	{
		system_fromJson(&game_map.systems[i], sj_array_get_nth(systems_arr, i));

	}

	for (i = 0; i < game_map.num_systems; i++)
	{
		curr_system = &game_map.systems[i];

		system_add_all_neighbors(curr_system, sj_array_get_nth(systems_arr, i));

	}

}

SJson* map_toJson()
{
	int i;

	SJson* map_json;
	SJson* systems_json;

	if (!game_map.systems)
	{
		slog("Cannot save NULL map to JSON"); return NULL;
	}

	map_json = sj_object_new();
	systems_json = sj_array_new();

	for (i = 0; i < game_map.num_systems; i++)
	{
		sj_array_append(systems_json, system_toJson(&game_map.systems[i]));
	}

	sj_object_insert(map_json, "Systems", systems_json);

	return map_json;
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

	game_map._hidden = 0;
}

void map_drawPaths()
{
	int i; int j;

	if (!game_map.systems)
	{
		slog("Cannot draw paths for unitialized map"); return;
	}

	if (game_map._hidden == 1)
	{
		return;
	}

	for (i = 0; i < game_map.num_systems; i++)
	{

		for (j = 0; j < game_map.systems[i].num_neighbor_systems; j++)
		{
			//NOTE: Maybe add and compare IDs? 'a' value in vector4d must be half of what we actually want because of twice-over redraws
			gf2d_draw_line
			(
				game_map.systems[i].ent_worldview->collidercircle->viewpos,
				game_map.systems[i].neighbor_systems[j]->ent_worldview->collidercircle->viewpos,
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


void map_reciever(void* self_void, Game_Event* gameEvent)
{


	if (!game_map.systems)
	{
		slog("Cannot free NULL map"); return;
	}

	if (gameEvent->event_command == (int)command_map_toggleVisibility)
	{
		if (game_map._hidden == 0)
		{
			map_hide();
		}

		else
		{
			map_show();
		}
	}
}

void map_hide()
{
	int i;

	if (!game_map.systems)
	{
		slog("Cannot free NULL map"); return;
	}

	for (i = 0; i < game_map.num_systems; i++)
	{
		game_map.systems[i].ent_worldview->_hidden = 1;
	}

	game_map._hidden = 1;

}

void map_show()
{
	int i;

	if (!game_map.systems)
	{
		slog("Cannot free NULL map"); return;
	}

	for (i = 0; i < game_map.num_systems; i++)
	{
		game_map.systems[i].ent_worldview->_hidden = 0;
	}

	game_map._hidden = 0;
}