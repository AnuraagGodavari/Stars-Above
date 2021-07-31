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

Map* map_fromJson(SJson* map_json);

SJson* map_toJson();


void map_init(Uint32 num_systems);

void map_free();

#endif
