#pragma once

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gfc_types.h"

#include "entity.h"

typedef struct
{

	Entity* ent;
	

} UI_Object;

typedef struct
{

	UI_Object* ui_list;

} UI_Arrangement;
