#include "simple_logger.h"

#include "starsabove_map.h"
#include "starsabove_system.h"

//JSON Handling

System* system_fromJson(System* self, SJson* self_json)
{
	int i;

	SJson* pos_json;
	Vector2D pos;

	SJson* planets_array;

	if (!self)
	{
		slog("Cannot load Star System to NULL address"); return NULL;
	}

	if (!self_json)
	{
		slog("Cannot load Star System from NULL json"); return NULL;
	}

	pos_json = sj_object_get_value(self_json, "worldpos");
	pos = vector2d(0, 0);

	//Convert the list elements in the position JSON array to coordinates
	sj_get_float_value(sj_array_get_nth(pos_json, 0), &pos.x);
	sj_get_float_value(sj_array_get_nth(pos_json, 1), &pos.y);

	system_init
	(
		self,
		sj_get_string_value(sj_object_get_value(self_json, "name")),
		pos,
		(Uint32) sj_array_get_count(sj_object_get_value(self_json, "neighbors")), 
		(Uint32) sj_array_get_count(sj_object_get_value(self_json, "planets"))
	);

	if (!self)
	{
		slog("Did not load planets"); return NULL;
	}

	planets_array = sj_object_get_value(self_json, "planets");

	for (i = 0; i < self->num_planets; i++)
	{
		if (self->planets[i] == NULL)
			self->planets[i] = planet_fromJson(sj_array_get_nth(planets_array, i), self);
	}

	return self;
}

void system_add_all_neighbors(System* self, SJson* self_json)
{
	int i; int j;
	SJson* neighbors_json;
	char curr_neighbor[128];
	Map* game_map;

	if (!self)
	{
		slog("Cannot add neighbors for a NULL system"); return;
	}

	if (!self_json)
	{
		slog("Cannot add neighbors for a system with a NULL json"); return;
	}

	neighbors_json = sj_object_get_value(self_json, "neighbors");

	game_map = get_map();

	//Iterate through the assigned neighbor systems
	for (i = 0; i < sj_array_get_count(neighbors_json); i++)
	{
		//Get the neighbor's name
		strcpy(curr_neighbor, sj_get_string_value(sj_array_get_nth(neighbors_json, i)));

		//Find the neighbor in the game map's list of systems
		for (j = 0; j < game_map->num_systems; j++)
		{

			if (strcmp(curr_neighbor, game_map->systems[j].name) == 0)
			{
				system_add_neighbor(self, &game_map->systems[j]);

				continue;
			}
		}
	}
}


//System Code

void system_init(System* self, char* name, Vector2D worldpos, Uint32 num_neighbor_systems, Uint32 num_planets)
{
	int i;

	if (!self)
	{
		slog("Cannot init NULL system"); return;
	}

	strcpy(self->name, name);

	self->num_neighbor_systems = num_neighbor_systems;

	self->neighbor_systems = malloc(sizeof(System*) * num_neighbor_systems);

	self->num_planets = num_planets;

	self->planets = malloc(sizeof(Planet*) * num_planets);

	self->ent = entity_init
	(
		gf2d_sprite_load_image("assets/images/gameobjects/star.png"),
		worldpos,
		COLL_CIRCLE,
		1,
		0
	);

	//Set all the neighboring systems' pointers to NULL for the time being
	for (i = 0; i < num_neighbor_systems; i++)
	{
		self->neighbor_systems[i] = NULL;
	}

	//Set all the planet pointers to NULL for the time being
	for (i = 0; i < num_neighbor_systems; i++)
	{
		self->planets[i] = NULL;
	}

	entity_add_clickevent
	(
		self->ent,
		game_event_new
		(
			(void*)self,
			(void*)self,
			(int)command_system_seePlanets,
			-1,
			system_reciever,
			NULL,
			NULL
		)
	);

}

void system_reciever(Game_Event* gameEvent)
{
	slog("SYSTEM RECIEVER");
}

void system_add_neighbor(System* self, System* neighbor)
{

	int i;

	if (!self)
	{
		slog("Cannot add neighbor to NULL system"); return;
	}

	if (!neighbor)
	{
		slog("Cannot add NULL neighbor to system"); return;
	}

	for (i = 0; i < self->num_neighbor_systems; i++)
	{

		if (self->neighbor_systems[i] == NULL)
		{
			self->neighbor_systems[i] = neighbor;

			return;
		}
	}
}

void system_add_planet(System* self, Planet* planet)
{

	int i;

	if (!self)
	{
		slog("Cannot add neighbor to NULL system"); return;
	}

	if (!planet)
	{
		slog("Cannot add NULL neighbor to system"); return;
	}

	for (i = 0; i < self->num_planets; i++)
	{

		if (self->planets[i] == NULL)
		{
			self->planets[i] = planet;

			return;
		}
	}
}

void system_free(System* self)
{
	entity_free(self->ent);

	memset(&self, 0, sizeof(System));
}

