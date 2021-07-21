#pragma once

#ifndef __GAMERESOURCES_H__
#define __GAMERESOURCES_H__

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gfc_types.h"

typedef struct Game_Event_t;

typedef void (*reciever_func)(void* self_void, struct Game_Event_t* game_event);

typedef struct Menu_State;

typedef struct
{
	void* actor;
	void* target;

	int event_command;

	int qty;

	short _gamewide;

	struct Menu_State (*menustate_generate) (void* self);

	reciever_func reciever;

	struct Game_Event_t* sub_event;


} Game_Event;

Game_Event* game_event_new(void* actor, void* target, int command, int qty, reciever_func* reciever);

void game_event_trigger(Game_Event* game_event);

void game_event_free(Game_Event* self);



#endif