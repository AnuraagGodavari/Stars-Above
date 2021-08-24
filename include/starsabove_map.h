#pragma once

#ifndef __SA_MAP_H__
#define __SA_MAP_H__

#include "simple_json.h"

#include "starsabove_system.h"

typedef enum 
{
	command_map_toggleVisibility
} 
map_commands;

typedef struct
{

	Uint32 num_systems;
	System* systems;

	short _hidden;

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


/*
* @brief Get a pointer to the game's map
*/
void map_reciever(void* self_void, Game_Event* gameEvent);

/*
* @brief Hide all the entities belonging to the map
*/
void map_hide();

/*
* @brief Show all the entities belonging to the map
*/
void map_show();





#endif
