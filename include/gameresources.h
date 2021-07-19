#pragma once

#ifndef __GAMERESOURCES_H__
#define __GAMERESOURCES_H__

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gfc_types.h"

#include "entity.h"

typedef struct Menu_State;

typedef struct Game_Event_t
{
	void* actor;
	void* target;

	int event_command;

	int qty;

	(Menu_State*) menustate_generate(void* self);

	(void) reciever(void* self);

	Game_Event_t* sub_event;


} Game_Event;

Game_Event* game_event_new(void* actor, void* target, int command, int qty);

void game_event_free(Game_Event* self);



#endif