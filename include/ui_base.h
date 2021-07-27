#pragma once

#ifndef __UI_BASE_H__
#define __UI_BASE_H__

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gfc_types.h"

#include "gameresources.h"

#include "entity.h"

typedef struct UI_Arrangement_s;
typedef struct UI_State;


typedef struct UI_Object_s
{

	Entity* ent;

	struct UI_Object_s* next;

	struct UI_Arrangement_s* parent;

} UI_Object;


typedef struct UI_Arrangement_s
{
	struct UI_State* parent;

	UI_Object* top;

	int size;

	int x_spacing;
	int y_spacing;

} UI_Arrangement;


typedef struct UI_State
{
	Uint32 num_arrs;
	UI_Arrangement** arrs;

	void* self_parent;
	uiState_generator self_generator;

	void* prev_parent;
	uiState_generator prev_generator;
} UI_State;

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
* @brief free a UI_Object
*/
void ui_object_free(UI_Object* self);

/*
* @brief Create a new UI_Arrangement
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
* @brief Free a UI_Arrangement
*/
void ui_arr_free(UI_Arrangement* self);

/*
* @brief Create a new UI State
*/
UI_State* ui_state_new(Uint32 num_arrs, void* self_parent, uiState_generator self_generator, void* prev_parent, uiState_generator prev_generator);

/*
* @brief Push Ui_Arr to the UI_State
*/
void ui_state_pushback(UI_State* self, UI_Arrangement* ui_arr);

/*
* @brief Free a UI_State
*/
void ui_state_free(UI_State* self);

#endif
