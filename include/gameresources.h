#pragma once

#ifndef __GAMERESOURCES_H__
#define __GAMERESOURCES_H__

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gfc_types.h"

typedef struct Game_Event_t;

typedef void (*reciever_func)(void* self_void, struct Game_Event_t* game_event);

typedef struct UI_State;

/*
* @brief Generate a new UI State (Menu)
* @param self The object you are making a UI State for
*/
typedef struct UI_State(*uiState_generator) (void* self, struct Game_Event_t* gameEvent_prev);

typedef struct
{
	void* actor;
	void* target;

	int event_command;

	int qty;

	short _gamewide;

	struct UI_State* uiState;

	uiState_generator uiState_next;

	uiState_generator uiState_previous;
	void* prev_actor;

	reciever_func reciever;

	struct Game_Event_t* sub_event;


} Game_Event;

Game_Event* game_event_new(void* actor, void* target, int command, int qty, reciever_func* reciever, struct UI_State* uiState, uiState_generator uiState_next);

void game_event_trigger(Game_Event* game_event);

void game_event_free(Game_Event* self);


struct UI_State* game_event_getUIState(Game_Event* self);

#endif