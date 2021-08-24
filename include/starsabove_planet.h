#pragma once

#ifndef __SA_PLANET_H__
#define __SA_PLANET_H__

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_types.h"

#include "gameresources.h"
#include "entity.h"
#include "ui_base.h"

typedef struct System;

typedef struct
{

	char name[128];

	struct System* parent;

	Sprite* sprite_systemview;

} Planet;

Planet* planet_fromJson(SJson* self_json, struct System* parent);

SJson* planet_toJson(Planet* self);


Planet* planet_init(char* name, struct System* parent, Sprite* sprite_systemview);

void planet_free(Planet* self);


UI_Object* planet_uiobj(Planet* self);

#endif
