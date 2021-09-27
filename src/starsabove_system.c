#include "simple_logger.h"

#include "game.h"
#include "ui_base.h"

#include "starsabove_map.h"
#include "starsabove_system.h"

//JSON Handling

System* system_fromJson(System* self, SJson* self_json)
{
	int i;

	SJson* pos_json;
	Vector2D pos;

	SJson* planets_array;

	if (!self)
	{
		slog("Cannot load Star System to NULL address"); return NULL;
	}

	if (!self_json)
	{
		slog("Cannot load Star System from NULL json"); return NULL;
	}

	pos_json = sj_object_get_value(self_json, "worldpos");
	pos = vector2d(0, 0);

	//Convert the list elements in the position JSON array to coordinates
	sj_get_float_value(sj_array_get_nth(pos_json, 0), &pos.x);
	sj_get_float_value(sj_array_get_nth(pos_json, 1), &pos.y);

	system_init
	(
		self,
		sj_get_string_value(sj_object_get_value(self_json, "name")),
		pos,
		(Uint32) sj_array_get_count(sj_object_get_value(self_json, "neighbors")), 
		(Uint32) sj_array_get_count(sj_object_get_value(self_json, "planets"))
	);

	if (!self)
	{
		slog("Did not load planets"); return NULL;
	}

	planets_array = sj_object_get_value(self_json, "planets");



	for (i = 0; i < self->num_planets; i++)
	{
			self->planets[i] = planet_fromJson(sj_array_get_nth(planets_array, i), self);
	}

	return self;
}

void system_add_all_neighbors(System* self, SJson* self_json)
{
	int i; int j;
	SJson* neighbors_json;
	char curr_neighbor[128];
	Map* game_map;

	if (!self)
	{
		slog("Cannot add neighbors for a NULL system"); return;
	}

	if (!self_json)
	{
		slog("Cannot add neighbors for a system with a NULL json"); return;
	}

	neighbors_json = sj_object_get_value(self_json, "neighbors");

	game_map = get_map();

	//Iterate through the assigned neighbor systems
	for (i = 0; i < sj_array_get_count(neighbors_json); i++)
	{
		//Get the neighbor's name
		strcpy(curr_neighbor, sj_get_string_value(sj_array_get_nth(neighbors_json, i)));

		//Find the neighbor in the game map's list of systems
		for (j = 0; j < game_map->num_systems; j++)
		{

			if (strcmp(curr_neighbor, game_map->systems[j].name) == 0)
			{
				system_add_neighbor(self, &game_map->systems[j]);

				continue;
			}
		}
	}
}

SJson* system_toJson(System* self)
{
	int i;

	SJson* system_json;

	SJson* worldpos_json;

	SJson* planets_json;
	SJson* neighbors_json;

	if (!self)
	{
		slog("Cannot save NULL system to Json"); return NULL;
	}

	system_json = sj_object_new();

	worldpos_json = sj_array_new();

	planets_json = sj_array_new();
	neighbors_json = sj_array_new();

	//Insert basic information

	sj_object_insert(system_json, "name", sj_new_str(self->name));
	
	sj_array_append(worldpos_json, sj_new_float(self->worldpos.x));
	sj_array_append(worldpos_json, sj_new_float(self->worldpos.y));
	sj_object_insert(system_json, "worldpos", worldpos_json);


	//Insert planets

	for (i = 0; i < self->num_planets; i++)
	{
		sj_array_append(planets_json, planet_toJson(self->planets[i]));
	}

	sj_object_insert(system_json, "planets", planets_json);


	//Insert neighbor names

	for (i = 0; i < self->num_neighbor_systems; i++)
	{

		sj_array_append(neighbors_json, sj_new_str(self->neighbor_systems[i]->name));
	}

	sj_object_insert(system_json, "neighbors", neighbors_json);

	return system_json;
}


//Setup Code

void system_clickevent(System* self)
{
	if (!self)
	{
		slog("Cannot init NULL system"); return;
	}

	entity_add_clickevent
	(
		self->ent_worldview,
		game_event_new
		(
			(void*)self,
			(void*)self,
			(int)command_map_toggleVisibility,
			-1,
			map_reciever,
			NULL,
			system_uiState,
			1
		)
	);
}

void system_init(System* self, char* name, Vector2D worldpos, Uint32 num_neighbor_systems, Uint32 num_planets)
{
	int i;

	if (!self)
	{
		slog("Cannot init NULL system"); return;
	}

	strcpy(self->name, name);

	self->num_neighbor_systems = num_neighbor_systems;

	self->neighbor_systems = malloc(sizeof(System*) * num_neighbor_systems);

	self->num_planets = num_planets;

	self->planets = malloc(sizeof(Planet*) * num_planets);

	//Load map sprite

	vector2d_copy(self->worldpos, worldpos);

	self->ent_worldview = entity_init
	(
		gf2d_sprite_load_image("assets/images/gameobjects/star.png"),
		worldpos,
		COLL_CIRCLE,
		1,
		0
	);

	//Load system view sprite

	self->sprite_systemview = gf2d_sprite_load_image("assets/images/gameobjects/star_systemview.png"); 

	//Set all the neighboring systems' pointers to NULL for the time being
	for (i = 0; i < num_neighbor_systems; i++)
	{
		self->neighbor_systems[i] = NULL;
	}

	//Set all the planet pointers to NULL for the time being
	for (i = 0; i < num_neighbor_systems; i++)
	{
		self->planets[i] = NULL;
	}

	system_clickevent(self);

}

void system_add_neighbor(System* self, System* neighbor)
{

	int i;

	if (!self)
	{
		slog("Cannot add neighbor to NULL system"); return;
	}

	if (!neighbor)
	{
		slog("Cannot add NULL neighbor to system"); return;
	}

	for (i = 0; i < self->num_neighbor_systems; i++)
	{

		if (self->neighbor_systems[i] == NULL)
		{
			self->neighbor_systems[i] = neighbor;

			return;
		}
	}
}

void system_add_planet(System* self, Planet* planet)
{

	int i;

	if (!self)
	{
		slog("Cannot add neighbor to NULL system"); return;
	}

	if (!planet)
	{
		slog("Cannot add NULL neighbor to system"); return;
	}

	for (i = 0; i < self->num_planets; i++)
	{

		if (self->planets[i] == NULL)
		{
			self->planets[i] = planet;

			return;
		}
	}
}

void system_free(System* self)
{
	entity_free(self->ent_worldview);

	memset(&self, 0, sizeof(System));
}


//System Code

void system_reciever(void* self_void, Game_Event* gameEvent)
{
	System* self;
	
	if (!self_void)
	{
		slog("Cannot recieve event for NULL system"); return;
	}

	if (!gameEvent)
	{
		slog("Cannot recieve NULL system event"); return;
	}

	self = (System*) self_void;

	if (gameEvent->event_command == (int)command_system_systemView)
	{
		
	}
}

UI_State* system_uiState(void* self_void, Game_Event* gameEvent_prev)
{
	int i;

	float planet_spacing; float planet_sprite_w_total;
	float textbox_spacing;

	char temp[128];

	System* self;

	UI_Object* curr_uiObj;
	UI_State* self_uiState;
	UI_Arrangement* curr_uiArr;

	Sprite* sprite_textbox;

	self = (System*)self_void;

	//Create new UI State
	self_uiState = ui_state_new(
		5,
		self_void,
		system_uiState,
		NULL,
		NULL
	);

	//Create UI Arr with just the system view visual (first moving it to the top-center)

	curr_uiArr = ui_arr_new(
		ui_object_new(
			entity_init
			(
				self->sprite_systemview,
				vector2d
				(
					get_gamedata()->screensize.x / 2 - self->sprite_systemview->frame_w / 2,
					get_gamedata()->screensize.y / 20
				),
				COLL_CIRCLE,
				0,
				0
			)
		),
		0,
		10
	);

	ui_state_pushback(self_uiState, curr_uiArr);

	//Planets UI Arr

	planet_sprite_w_total = 0;

	//Temporarily set planet_spacing to the combined frame width of all the planet sprites

	for (i = 0; i < self->num_planets; i++)
	{
		planet_sprite_w_total += self->planets[i]->sprite_systemview->frame_w;
	}

	planet_spacing = (get_gamedata()->screensize.x - planet_sprite_w_total) / (self->num_planets + 1);

	curr_uiArr = ui_arr_new(
		NULL,
		planet_spacing,
		0
	);

	//Add the first planet

	ui_arr_add(curr_uiArr, planet_uiobj(self->planets[0]));

	//Place the first planet at the necessary coordinates to have a visually consistent ui_array

	vector2d_copy(
		curr_uiArr->top->ent->pos, 
		vector2d(
			planet_spacing, 
			(get_gamedata()->screensize.y * 9/10) - (planet_sprite_w_total / self->num_planets)
		)
	);

	for (i = 1; i < self->num_planets; i++)
	{
		ui_arr_add(curr_uiArr, planet_uiobj(self->planets[i]));
	}

	ui_state_pushback(self_uiState, curr_uiArr);


	//Planet Names UI Arr, plus first element

	sprite_textbox = gf2d_sprite_load_image("assets/images/ui/textbox_brown.png");

	textbox_spacing = (get_gamedata()->screensize.x - (self->num_planets * sprite_textbox->frame_w)) / (self->num_planets + 1);

	curr_uiArr = ui_arr_new(
		ui_object_new
		(
			entity_init(sprite_textbox, vector2d(0, 0), COLL_BOX, 0, 0)
		),
		textbox_spacing,
		0
	);

	entity_add_text(curr_uiArr->top->ent, self->planets[0]->name, font_load("assets/fonts/futura light bt.ttf", 16));

	//Place the first textbox at the necessary coordinates to have a visually consistent ui_array

	vector2d_copy(
		curr_uiArr->top->ent->pos,
		vector2d(
			textbox_spacing,
			(get_gamedata()->screensize.y * 19 / 20) - sprite_textbox->frame_h
		)
	);

	for (i = 1; i < self->num_planets; i++)
	{
		curr_uiObj = ui_object_new
		(
			entity_init(sprite_textbox, vector2d(0, 0), COLL_BOX, 0, 0)
		);

		ui_arr_add(
			curr_uiArr,
			curr_uiObj
		);

		entity_add_text(curr_uiObj->ent, self->planets[i]->name, font_load("assets/fonts/futura light bt.ttf", 16));
	}

	ui_state_pushback(self_uiState, curr_uiArr);

	return self_uiState;
}

