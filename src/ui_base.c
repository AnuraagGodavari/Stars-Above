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

void ui_object_click_reciever(void* self_void)
{
	UI_Object* self;

	if (self == NULL)
	{
		slog("Cannot perform click actions on a NULL UI Object!");
	}

	self = (UI_Object*)self_void;
}

void ui_object_free(UI_Object* self)
{
	if (self->ent) entity_free(self->ent);

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

	curr = self->top;
	next = curr->next;

	while (curr != NULL)
	{
		ui_object_free(curr);

		curr = next;
		next = curr->next;
	}

	memset(&self, 0, sizeof(UI_Arrangement));
}