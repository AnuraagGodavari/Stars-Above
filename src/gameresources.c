#include "simple_logger.h"

#include "gameresources.h"

#include "game.h"

Game_Event* game_event_new(void* actor, void* target, int command, int qty, reciever_func* reciever, UI_State* uiState, uiState_generator uiState_next)
{
	Game_Event* self;

	if (!actor)
	{
		slog("Cannot make game event with NULL actor"); return NULL;
	}

	self = malloc(sizeof(Game_Event));

	self->actor = actor;

	self->target = target;

	self->event_command = command;

	self->qty = qty;

	self->reciever = reciever;

	self->uiState = uiState;
	self->uiState_next = uiState_next;

	if (target) { self->_gamewide = 0; }

	else { self->_gamewide = 1; }

	self->sub_event = NULL;
	self->prev_event = NULL;

	return self;

}

Game_Event* game_event_copy(Game_Event* self)
{
	Game_Event* copy;

	if (!self)
	{
		slog("Cannot make copy of NULL game event"); return NULL;
	}

	copy = game_event_new
	(
		self->actor,
		self->target,
		self->event_command,
		self->qty,
		self->reciever,
		self->uiState,
		self->uiState_next
	);

	if (self->sub_event)
		copy->sub_event = game_event_copy(self->sub_event);

	if (self->prev_event)
		copy->prev_event = game_event_copy(self->prev_event);

	return copy;
}

void game_event_trigger(Game_Event* self)
{

	UI_State* new_uistate;

	if (!self)
	{
		slog("Cannot trigger NULL game event"); return;
	}

	if (self->_gamewide)
	{
		game_recieve_event(self);
	}

	else
	{
		self->reciever(self->target, self);
	}

	if (self->uiState_next)
	{
		new_uistate = self->uiState_next(self->target, self);

		new_uistate->prev_game_event = game_event_copy(self);

		game_set_ui_state(new_uistate);
	}

	if (self->uiState)
		ui_state_free(self->uiState);
	
}

void game_event_free(Game_Event* self)
{

	if (!self)
	{
		slog("Cannot free NULL Game Event"); return;
	}

	if (self->sub_event)
		game_event_free(self->sub_event);

	if (self->prev_event)
		game_event_free(self->prev_event);

	memset(&self, 0, sizeof(Game_Event));
}

UI_State* game_event_getUIState(Game_Event* self)
{
	return self->uiState;
}