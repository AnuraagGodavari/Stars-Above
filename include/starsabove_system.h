#pragma once

#ifndef __SA_SYSTEM_H__
#define __SA_SYSTEM_H__

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_types.h"

#include "gameresources.h"

#include "entity.h"

typedef struct System_s
{

	char name[128];
	int id;

	Vector2D worldpos;
	Entity* ent;

	Uint32 num_neighbor_systems;
	struct System_s** neighbor_systems;

} System;

/*
* @brief Load a system from a JSON object
*/
System* system_fromJson(System* self, SJson* self_json);

/*
* @brief Add all of a system's neighbors according to a JSON object
*/
void system_add_all_neighbors(System* self, SJson* self_json);

/*
* @brief Save the system to a JSON object
*/
SJson* system_toJson(System* self);


/*
* @brief Initialize a system to an address within the game's map
* @param self NULL address where we want this system to be in memory
*/
System* system_init(System* self, char* name, Vector2D worldpos, Uint32 num_neighbor_systems);

/*
* @brief Add a neighboring system to a system
*/
void system_add_neighbor(System* self, System* neighbor);

/*
* @brief Free a star system
*/
void system_free(System* system);

#endif