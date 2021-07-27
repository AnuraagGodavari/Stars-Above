#include "simple_logger.h"

#include "ui_base.h"

//UI Object Code

UI_Object* ui_object_new(Entity* entity)
{

	UI_Object* self;
	
	self = malloc(sizeof(UI_Object));

	if (entity) self->ent = entity;

	self->next = NULL;

	return self;
}

void ui_object_add(UI_Object* self, UI_Object* new_ui, int x_spacing, int y_spacing)
{
	if (!self)
	{
		slog("Cannot add UI object to null UI object"); return;
	}

	if (!new_ui)
	{
		slog("Cannot add null UI object to UI object"); return;
	}

	if (self->next != NULL)
	{
		ui_object_add(self->next, new_ui, x_spacing, y_spacing);

		return;
	}

	vector2d_copy(new_ui->ent->pos, self->ent->pos);

	if (x_spacing != 0)
	{
		new_ui->ent->pos.x += (float)new_ui->ent->sprite->frame_w + (float)x_spacing;
	}

	if (y_spacing != 0)
	{
		new_ui->ent->pos.y += (float)new_ui->ent->sprite->frame_h + (float)y_spacing;
	}

	if (new_ui->ent->collidercircle)
	{
		vector2d_copy(new_ui->ent->collidercircle->viewpos, new_ui->ent->pos);
	}
	
	if (new_ui->ent->colliderbox)
	{
		vector2d_copy(new_ui->ent->colliderbox->viewpos, new_ui->ent->pos);
	}

	new_ui->parent = self->parent;

	self->next = new_ui;
}

void ui_object_free(UI_Object* self)
{
	if (self->ent) entity_free(self->ent);

	if (self->next) ui_object_free(self->next);

	memset(&self, 0, sizeof(UI_Object));
}

//UI Arrangement Code

UI_Arrangement* ui_arr_new(UI_Object* root, int x_spacing, int y_spacing)
{

	UI_Arrangement* self;

	self = malloc(sizeof(UI_Arrangement));

	self->x_spacing = x_spacing;
	self->y_spacing = y_spacing;

	if (!root) { self->top = NULL; self->size = 0; }

	else { self->top = root; self->top->parent = self; self->size = 1; }

	return self;
}

void ui_arr_add(UI_Arrangement* self, UI_Object* new_ui)
{

	if (!self)
	{
		slog("Cannot add UI Object to null UI Arrangement"); return;
	}

	if (!new_ui)
	{
		slog("Cannot add null UI Object to UI Arrangement"); return;
	}

	if (self->top == NULL)
	{
		self->top = new_ui;
	}

	else
	{
		ui_object_add(self->top, new_ui, self->x_spacing, self->y_spacing);
	}

	self->size++;
}

void ui_arr_free(UI_Arrangement* self)
{
	UI_Object* curr;
	UI_Object* next;

	if (!self)
	{
		slog("Cannot free null UI Arrangement"); return;
	}

	ui_object_free(self->top);

	/*
	curr = self->top;
	next = curr->next;
	while (curr != NULL)
	{
		ui_object_free(curr);

		curr = next;
		next = curr->next;
	}
	*/

	memset(&self, 0, sizeof(UI_Arrangement));
}

//UI State Code

UI_State * ui_state_new(Uint32 num_arrs, void* self_parent, uiState_generator self_generator, void* prev_parent, uiState_generator prev_generator)
{
	int i;
	UI_State* self;

	if (num_arrs <= 0)
	{
		slog("Cannot make a UI State that holds 0 arrangements"); return NULL;
	}

	/*
	if (!self_parent)
	{
		slog("Cannot add NULL parent pointer to UI State"); return NULL;
	}
	*/

	if (!self_generator)
	{
		slog("Cannot add NULL parent generator to UI State"); return NULL;
	}

	self = malloc(sizeof(UI_State));

	self->num_arrs = num_arrs;
	self->arrs = malloc(sizeof(UI_Arrangement*) * num_arrs);

	self->self_parent = self_parent;
	self->self_generator = self_generator;

	self->prev_parent = prev_parent;
	self->prev_generator = prev_generator;

	for (i = 0; i < self->num_arrs; i++)
	{
		self->arrs[i] = NULL;
	}

	return self;
}

void ui_state_pushback(UI_State* self, UI_Arrangement* ui_arr)
{
	int i;
	
	if (!self)
	{
		slog("Cannot add UI_Arrangement to NULL UI_State"); return;
	}

	if (!ui_arr)
	{
		slog("Cannot add NULL UI_Arrangement to UI_State"); return;
	}

	for (i = 0; i < self->num_arrs; i++)
	{
		if (self->arrs[i] == NULL)
		{
			self->arrs[i] = ui_arr;
			return;
		}
	}

	slog("Not enough spaces for new UI Arrangement"); return;
}

void ui_state_free(UI_State* self)
{
	int i;

	if (!self)
	{
		slog("Cannot free NULL UI_State"); return;
	}

	for (i = 0; i < self->num_arrs; i++)
	{
		if (self->arrs[i] != NULL)
		{
			ui_arr_free(self->arrs[i]);
		}
	}
	memset(&self, 0, sizeof(UI_State));

}