#pragma once

#ifndef __SA_GAME_H_
#define __SA_GAME_H_

#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gfc_types.h"

#include "gameresources.h"

#include "camera.h"
#include "text.h"

#include "entity.h"
#include "ui_base.h"

typedef enum game_commands
{
	command_game_TEST
};

typedef struct 
{

	Vector2D screensize;

	Entity* hovering_ent;

	UI_State* uiState_curr;

} Game_Data;

/* Main game stuff */

void game_prepare();

void game_load(char* savefile);

void game_loop();

/* Other game stuff */

void game_recieve_event(Game_Event* event);

void game_set_ui_state(UI_State* ui_state);

/* Mouse Stuff */

void mouse_update(Sprite* idle, Sprite* hover, Sprite* drag);

/* Controller Stuff */

void process_keys();

void sdl_event();

#endif