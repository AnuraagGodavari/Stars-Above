#pragma once

#ifndef __SA_GAME_H_
#define __SA_GAME_H_

#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gfc_types.h"

#include "camera.h"
#include "text.h"

#include "entity.h"
#include "ui_base.h"

typedef struct 
{

	Vector2D screensize;

	Entity* hovering_ent;

} Game_Data;

void game_prepare();

void game_load(char* savefile);

void game_loop();

/* Mouse Stuff */

void mouse_update(Sprite* idle, Sprite* hover, Sprite* drag);

/* Controller Stuff */

void process_keys();

void sdl_event();

#endif