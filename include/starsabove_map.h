#pragma once

#ifndef __SA_MAP_H__
#define __SA_MAP_H__

#include "simple_json.h"

#include "starsabove_system.h"

typedef struct
{

	Uint32 num_systems;
	System* systems;

} Map;

/*
* @brief Load the game's map from a JSON object
*/
Map* map_fromJson(SJson* map_json);

/*
* @brief Save the game's map to a JSON object
*/
SJson* map_toJson();


/*
* @brief Initialize the game's map
*/
void map_init(Uint32 num_systems);

/*
* @brief Render the lines between neighboring locations
*/
void map_drawPaths();

/*
* @brief Free the game's map
*/
void map_free();

/*
* @brief Get a pointer to the game's map
*/
Map* get_map();

#endif
