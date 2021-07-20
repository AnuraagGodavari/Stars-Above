#pragma once

#ifndef __SA_ENTITY_H__
#define __SA_ENTITY_H__

#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gfc_types.h"

#include "collision.h"
#include "camera.h"
#include "text.h"

/*typedef enum Parent_Type
{

	ENT_UI,
	ENT_STAR,
	ENT_PLANET

} Parent_Type;*/

typedef struct Entity_s
{

	short _inuse;

	Sprite* sprite;

	Vector2D pos;
	short ingame_object;

	Collider_Box* colliderbox;
	Collider_Circle* collidercircle;

	char text[256];
	Font* font;
	Vector2D textoffset;

	short _hidden;
	short _clickable;

	void* parent;

	void (*draw) (struct Entity_s* self, Camera* cam);

	//Points to a function belonging to Entity->parent, which will convert the void pointer to a type pointer.
	void (*onClick) (void* self_void);

	void (*free) (struct Entity_s* self);

} Entity;

/*
* @brief Initialize the entity manager
*/
void entity_manager_init(Uint32 max_entities, Uint32 max_ui);

/*
* @brief Free the entity manager
*/
void entity_manager_free();


/*
* @brief Check and see if an entity is visible and clickable
* @return The entity that the mouse is hovering over
*/
Entity* entities_clickable(Vector2D mousepos);

/*
* @brief Draw all entities in the entity manager
*/
void all_entities_draw();


/*
* @brief Initialize a visualizable entity.
* @param collidertype What kind of collider (circle or square)
* @param is_ingameobject If the entity belongs to something that exists in the ingame world (otherwise it is something like UI)
*/
Entity* entity_init(Sprite* sprite, Vector2D pos, enum_collider_type collidertype, short is_ingameobject, short is_hidden, short is_clickable);

/*
* @brief Add text to an entity with a particular position.
* @param position Relative position of the text to the entity
*/
void entity_add_text_pos(Entity* self, char text[256], Font* font, Vector2D pos);

/*
* @brief Add text to an entity with an automatically determined position.
*/
void entity_add_text(Entity* self, char text[256], Font* font);

/*
* @brief Draw the entity to the screen relative to the position of the camera
*/
void entity_draw(Entity* self);

/*
* @brief Draw the entity to the screen relative to the position of the camera
*/
int entity_clickable(Entity* self, Vector2D mousepos);

/*
* @brief Free the entity struct
*/
void entity_free(Entity* self);

#endif