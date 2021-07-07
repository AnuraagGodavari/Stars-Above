#pragma once

#ifndef __SA_GAME_H_
#define __SA_GAME_H_

#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gfc_types.h"

#include "camera.h"

#include "entity.h"

typedef struct 
{

	Vector2D screensize;
	Camera* camera;

	Entity* hovering_ent;

} Game_Data;

void game_prepare();

void game_load(char* savefile);

void game_loop();

/* Mouse Stuff */

Bool mouse_clickable();

void mouse_update(Sprite* idle, Sprite* hover, Sprite* drag);

/* Controller Stuff */

void process_keys();

#endif