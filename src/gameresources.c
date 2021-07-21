#include "simple_logger.h"

#include "gameresources.h"

#include "game.h"

Game_Event* game_event_new(void* actor, void* target, int command, int qty, reciever_func* reciever)
{
	Game_Event* self;

	if (!actor)
	{
		slog("Cannot make game event with NULL actor"); return;
	}

	if (!reciever)
	{
		slog("Cannot make game event with NULL reciever function"); return;
	}

	self = malloc(sizeof(Game_Event));

	self->actor = actor;

	self->target = target;

	self->event_command = command;

	self->qty = qty;

	self->reciever = reciever;

	if (target) self->_gamewide = 1;

	else self->_gamewide = 0;

}

void game_event_trigger(Game_Event* self)
{

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
	
}