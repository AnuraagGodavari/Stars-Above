#include "starsabove_system.h"
#include "starsabove_planet.h"

#include "simple_logger.h"

Planet* planet_fromJson(SJson* self_json, System* parent)
{
	Planet* self;

	if (!self_json)
	{
		slog("Cannot load planet from NULL Json"); return NULL;
	}

	if (!parent)
	{
		slog("Cannot load planet with NULL parent system"); return NULL;
	}

	self = planet_init
	(
		sj_get_string_value(sj_object_get_value(self_json, "name")),
		parent
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


Planet* planet_init(char* name, System* parent)
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

	return self;
	
}

void planet_free(Planet* self)
{

}