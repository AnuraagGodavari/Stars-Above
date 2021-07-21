#pragma once

#ifndef __UI_BASE_H__
#define __UI_BASE_H__

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gfc_types.h"

#include "entity.h"

typedef struct UI_Arrangement_s;

typedef struct UI_Object_s
{

	Entity* ent;

	struct UI_Object_s* next;

	struct UI_Arrangement_s* parent;

} UI_Object;

typedef struct UI_Arrangement_s
{

	UI_Object* top;

	int size;

	int x_spacing;
	int y_spacing;

} UI_Arrangement;

/*
* @brief create a new UI_Object
* @param entity (Optional) Entity to tie to the UI object.
*/
UI_Object* ui_object_new(Entity* entity);

/*
* @brief Add a new UI_Object into self->next
* @param new_ui UI_Object to add
*/
void ui_object_add(UI_Object* self, UI_Object* new_ui, int x_spacing, int y_spacing);

/*
* @brief Do actions on a UI Object being clicked
* @param self_void A void pointer to the UI object
*/
void ui_object_click_reciever(void* self_void);

/*
* @brief free a UI_Object
*/
void ui_object_free(UI_Object* self);

/*
* @brief create a new UI_Arrangement
*/
UI_Arrangement* ui_arr_new(UI_Object* root, int x_spacing, int y_spacing);

/*
* @brief Add a new UI Object to the arrangement
*/
void ui_arr_add(UI_Arrangement* self, UI_Object* new_ui);

/*
* @brief Used for iterating through a UI Arrangement's objects
*/
//UI_Object* ui_arr_list(UI_Arrangement* self, int index);

/*
* @brief free a UI_Arrangement
*/
void ui_arr_free(UI_Arrangement* self);

#endif
