#include "starsabove_system.h"
#include "starsabove_planet.h"

#include "simple_logger.h"

Planet* planet_fromJson(SJson* self_json, System* parent)
{
	Planet* self;

	char spritename[128];

	if (!self_json)
	{
		slog("Cannot load planet from NULL Json"); return NULL;
	}

	if (!parent)
	{
		slog("Cannot load planet with NULL parent system"); return NULL;
	}

	sprintf(spritename, "assets/images/gameobjects/planet_%s.png", sj_get_string_value(sj_object_get_value(self_json, "type")));

	self = planet_init
	(
		sj_get_string_value(sj_object_get_value(self_json, "name")),
		parent,
		gf2d_sprite_load_image(spritename)
	);

	return self;
}

SJson* planet_toJson(Planet* self)
{
	SJson* planet_json;

	if (!self)
	{
		slog("Cannot save NULL planet to Json"); return NULL;
	}

	planet_json = sj_object_new();

	sj_object_insert(planet_json, "name", sj_new_str(self->name));

	return planet_json;
}


Planet* planet_init(char* name, System* parent, Sprite* sprite_systemview)
{
	Planet* self;

	if (!name)
	{
		slog("Cannot init planet with NULL name"); return NULL;
	}

	if (!parent)
	{
		slog("Cannot init planet with NULL parent system"); return NULL;
	}

	self = malloc(sizeof(Planet));

	strcpy(self->name, name);

	self->parent = parent;

	self->sprite_systemview = sprite_systemview;

	return self;
	
}

void planet_free(Planet* self)
{

}


UI_Object* planet_uiobj(Planet* self)
{
	if (!self)
	{
		slog("Cannot return UI Object for NULL Planet"); return NULL;
	}

	return ui_object_new
	(
		entity_init
		(
			self->sprite_systemview,
			vector2d
			(
				0,
				0
			),
			COLL_CIRCLE,
			0,
			0
		)
	);
}