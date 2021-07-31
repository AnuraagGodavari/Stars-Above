#include "simple_logger.h"

#include "starsabove_map.h"
#include "starsabove_system.h"

//JSON Handling

System* system_fromJson(System* self, SJson* self_json)
{
	SJson* pos_json;
	Vector2D pos;

	if (!self_json)
	{
		slog("Cannot load Star System from NULL json"); return;
	}

	pos_json = sj_object_get_value(self_json, "worldpos");
	pos = vector2d(0, 0);

	sj_get_float_value(sj_array_get_nth(pos_json, 0), &pos.x);
	sj_get_float_value(sj_array_get_nth(pos_json, 1), &pos.y);

	system_init
	(
		self,
		sj_get_string_value(sj_object_get_value(self_json, "name")),
		pos,
		(Uint32) sj_array_get_count(sj_object_get_value(self_json, "neigbors"))
	);

}

void system_add_all_neighbors(System* self, SJson* self_json)
{
	if (!self)
	{
		slog("Cannot add neighbors for a NULL system"); return;
	}

	if (!self_json)
	{
		slog("Cannot add neighbors for a system with a NULL json"); return;
	}

	slog("UNFINISHED!");
}


//System Code

System* system_init(System* self, char* name, Vector2D worldpos, Uint32 num_neighbor_systems)
{

	if (!self)
	{
		slog("Cannot init NULL system");
	}

	strcpy(self->name, name);

	self->num_neighbor_systems = num_neighbor_systems;
	self->neighbor_systems = malloc(sizeof(System*) * num_neighbor_systems);

	self->ent = entity_init
	(
		gf2d_sprite_load_image("assets/images/gameobjects/star.png"),
		worldpos,
		COLL_CIRCLE,
		1,
		0
	);
}

void system_add_neighbor(System* self, System* neighbor)
{

}

void system_free(System* system)
{

}

