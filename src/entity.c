#include "simple_logger.h"

#include "entity.h"

typedef struct
{
	short _initialized;

	Uint32 max_entities;
	Entity* entities;

} Entity_Manager;

Entity_Manager entity_manager = { 0 };

//Entity manager code

void entity_manager_init(Uint32 max_entities)
{
	int i;

	if (max_entities <= 0)
	{
		slog("Entity Manager cannot be initialized with non-positive value"); return;
	}

	entity_manager.max_entities = max_entities;

	entity_manager.entities = malloc(sizeof(Entity) * max_entities);

	for (i = 0; i < max_entities; i++) entity_manager.entities[i]._inuse = 0;

	entity_manager._initialized = 1;

	slog("Initialized Entity Manager");
}

void entity_manager_free()
{
	if (entity_manager.entities != NULL) {
		free(entity_manager.entities);
	}

	if (entity_manager._initialized == 0)
	{
		slog("Entity Manager does not exist"); return;
	}

	memset(&entity_manager, 0, sizeof(Entity_Manager));
	slog("Entity system freed");
}

//Multiple Entities

Entity* entity_new()
{
	int i;

	if (entity_manager._initialized == 0)
	{
		slog("Entity Manager does not exist"); return;
	}

	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entities[i]._inuse == 0)
		{
			return &entity_manager.entities[i];
		}
	}

	return NULL;
}

Entity* entities_clickable(Vector2D mousepos)
{
	int i;

	if (entity_manager._initialized == 0)
	{
		slog("Entity Manager does not exist"); return NULL;
	}

	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entities[i]._inuse != 0)
		{
			if (entity_manager.entities[i]._hidden == 0)
			{
				if (entity_clickable(&entity_manager.entities[i], mousepos))
				{
					return &entity_manager.entities[i];
				}
			}
		}
	}

	return NULL;
}

void entities_draw()
{
	int i;

	if (entity_manager._initialized == 0)
	{
		slog("Entity Manager does not exist"); return;
	}

	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entities[i]._inuse != 0)
		{
			entity_draw(&entity_manager.entities[i]);
		}
	}
}

//Individual Entity struct code

Entity* entity_init(Sprite* sprite, Vector2D pos, enum_collider_type collidertype, short is_ingameobject, short is_hidden)
{
	int i;

	Entity* entity = NULL;

	if (!sprite)
	{
		slog("Cannot create entity without a sprite!"); return NULL;
	}


	//Allocate the new entity and assign its sprite and position

	entity = entity_new();

	if (entity == NULL)
	{
		slog("Entity could not be initialized"); return;
	}

	entity->sprite = sprite;

	vector2d_copy(entity->pos, pos);


	//Set whether or not this entity is an ingame object or part of the ui.

	if (is_ingameobject == 0) { entity->ingame_object = 0; }

	else { entity->ingame_object = 1; }


	//Set whether or not this entity is hidden

	if (is_hidden == 0) { entity->_hidden = 0; }

	else { entity->_hidden = 1; }


	//Set the entity collider type

	if (collidertype == COLL_BOX)
	{
		//Set the collider to be a box with the same position as this entity and a length and width equal to those of its sprite.

		entity->colliderbox = box_new();

		vector2d_copy(entity->colliderbox->worldpos, entity->pos);
		vector2d_copy(entity->colliderbox->viewpos, entity->pos);

		vector2d_copy(entity->colliderbox->size,  vector2d
			(
				entity->sprite->frame_w,
				entity->sprite->frame_h
			)
		);

		entity->collidercircle = NULL;
	}

	else if (collidertype == COLL_CIRCLE)
	{
		//Set the collider to be a circle with a radius equal to the length or width of the sprite (whichever is larger), and a position where the center of the sprite would be.

		entity->collidercircle = circle_new();

		vector2d_add(
			entity->collidercircle->worldpos, 
			entity->pos, 
			vector2d(
				entity->sprite->frame_w / 2, 
				entity->sprite->frame_h / 2
			)
		);

		vector2d_copy(entity->collidercircle->viewpos, entity->collidercircle->worldpos);
		
		entity->collidercircle->radius = max(entity->sprite->frame_w, entity->sprite->frame_h) * 0.5;

		entity->colliderbox = NULL;
	}


	entity->draw = NULL;

	entity->font = NULL;

	entity->_inuse = 1;

	return entity;
}

void entity_add_text_pos(Entity* self, char text[256], Font* font, Vector2D pos)
{
	if (!text)
	{
		slog("Cannot add NULL text to an entity"); return;
	}

	if (!font)
	{
		slog("Cannot add text with NULL font to an entity"); return;
	}

	strcpy(self->text, text);
	self->font = font;
	vector2d_copy(self->textoffset, pos);

}

void entity_add_text(Entity* self, char text[256], Font* font)
{
	if (!text)
	{
		slog("Cannot add NULL text to an entity"); return;
	}

	if (!font)
	{
		slog("Cannot add text with NULL font to an entity"); return;
	}

	entity_add_text_pos(self, text, font,
		vector2d(
			self->sprite->frame_h * 0.2,
			(self->sprite->frame_h - font->ptsize) / 2
		));
}

void entity_draw(Entity* self)
{

	//Vector2D adjustedpos = *vector2d_new();

	Camera *cam = camera_get();

	if (!self)
	{
		slog("Cannot draw NULL entity!"); return;
	}
	
	if (self->_hidden == 1) return;

	//Custom Draw
	if (self->draw) { self->draw(self, cam); }

	else {

		if (self->ingame_object != 0)
		{

			if (!cam)
			{
				slog("Cannot draw ingame object without a camera!"); return;
			}

			if (self->colliderbox)
			{
				vector2d_copy(self->colliderbox->viewpos, vector2d(self->colliderbox->worldpos.x - cam->pos.x, self->colliderbox->worldpos.y - cam->pos.y));
			}

			if (self->collidercircle)
			{
				vector2d_copy(self->collidercircle->viewpos, vector2d(self->collidercircle->worldpos.x - cam->pos.x, self->collidercircle->worldpos.y - cam->pos.y));
			}

			if (camera_inview(self->collidercircle, self->colliderbox))
			{

				//Adjusted position is the entity's position relative to the camera
				//vector2d_sub(adjustedpos, self->pos, cam->pos);

				gf2d_sprite_draw
				(
					self->sprite,
					vector2d(self->pos.x - cam->pos.x, self->pos.y - cam->pos.y),
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					0
				);

				if (self->font)
				{
					font_render(self->font, self->text,
						vector2d(
							self->pos.x - cam->pos.x + self->textoffset.x,
							self->pos.y - cam->pos.y + self->textoffset.y
						),
						gfc_color(1, 1, 1, 0)
					);
				}


			}
		}

		else
		{

			gf2d_sprite_draw
			(
				self->sprite,
				self->pos,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				0
			);

			if (self->font)
			{
				font_render(self->font, self->text,
					vector2d(
						self->pos.x + self->textoffset.x,
						self->pos.y + self->textoffset.y
					),
					gfc_color(1, 1, 1, 0)
				);
			}
		}
	}

}

int entity_clickable(Entity* self, Vector2D mousepos)
{
	if (!self)
	{
		slog("Cannot check if NULL entity is clickable!"); return 0;
	}

	if (self->colliderbox)
	{
		if (box_clickable(self->colliderbox, mousepos))
		{
			return 1;
		}
	}

	else if (self->collidercircle)
	{
		if (circle_clickable(self->collidercircle, mousepos))
		{
			return 1;
		}
	}

	return 0;
}

void entity_free(Entity* self)
{

	if (!self)
	{
		slog("Cannot free NULL entity!"); return;
	}

	if (self->colliderbox != NULL)
	{
		box_free(self->colliderbox);
	}

	if (self->collidercircle != NULL)
	{
		circle_free(self->collidercircle);
	}

	self->_inuse = 0;

	memset(&self, 0, sizeof(Entity));

}