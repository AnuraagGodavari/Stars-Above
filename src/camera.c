#include "simple_logger.h"

#include "camera.h"

Camera game_cam = { 0 };

Camera* camera_get()
{
	return &game_cam;
}

Camera* camera_init(Vector2D pos, Vector2D size, Vector2D movementspeed)
{

	if (&pos == NULL)
	{
		slog("Cannot create camera with position NULL"); return NULL;
	}

	if (&size == NULL)
	{
		slog("Cannot create camera with size NULL"); return NULL;
	}

	if (&movementspeed == NULL)
	{
		slog("Cannot create camera with movement speed NULL"); return NULL;
	}

	vector2d_copy(game_cam.pos, pos);
	vector2d_copy(game_cam.size, size);
	vector2d_copy(game_cam.movementspeed, movementspeed);

	game_cam.collider = box_new();
	vector2d_copy(game_cam.collider->worldpos, pos);
	vector2d_copy(game_cam.collider->size, size);

	atexit(camera_free);
}

Parallax_Layer* parallax_init(Sprite* sprite, Vector2D pos, float drag)
{
	Parallax_Layer* parallax;

	parallax = malloc(sizeof(Parallax_Layer));

	parallax->sprite = sprite;
	
	parallax->collider = box_new();

	vector2d_copy(parallax->collider->worldpos, pos);
	vector2d_copy(parallax->collider->size, vector2d((float)sprite->frame_w, (float)sprite->frame_h) );

	parallax->drag = drag;

	return parallax;
}

void camera_parallax_draw(Parallax_Layer* parallax)
{
	//A temporary collider with the size of the parallax and origin of the camera
	Collider_Box tempcollider;

	//Vector with number of times we need to draw parallax sprite in x and y direction
	Vector2D itersvec;

	Vector2D temp;

	int i, j;
	float px_x, px_y;

	vector2d_copy(tempcollider.worldpos, game_cam.collider->worldpos);

	//If the parallax is dragging behind, multiply the camera's position by the drag amount to compensate.
	tempcollider.worldpos.x *= parallax->drag;
	tempcollider.worldpos.y *= parallax->drag;

	vector2d_copy(tempcollider.size, parallax->collider->size);

	px_x = parallax->collider->size.x;
	px_y = parallax->collider->size.y;

	i = 0;

	//First, check if we need to reposition. If the camera is not colliding with the parallax
	//Increase or decrease the parallax center's x and y by the parallax's x and y size until it once again intersects with the temp collider.
	if (!box_detectCollision(&tempcollider, parallax->collider))
	{

		if (parallax->collider->worldpos.x < tempcollider.worldpos.x)
		{ 
			parallax->collider->worldpos.x += px_x;
		}

		else if (abs(parallax->collider->worldpos.x - tempcollider.worldpos.x) > 0.001)
		{ 
			parallax->collider->worldpos.x -= px_x;
		}

		if (parallax->collider->worldpos.y < tempcollider.worldpos.y)
		{ 
			parallax->collider->worldpos.y += px_y;
		}

		else if (abs(parallax->collider->worldpos.y - tempcollider.worldpos.y) > 0.001)
		{ 
			parallax->collider->worldpos.y -= px_y;
		}

	}

	//Dimensions of itersvec are the amount of times that the size of the collider in one dimension go into the distance between parallax center and camera in that direction.
	//Add 2 to each side: one to overlap beyond the camera in either direction in each dimension.
	vector2d_copy(
		itersvec, 
		vector2d
		(
			(float) ((int) (abs(game_cam.collider->size.x - parallax->collider->size.x) / parallax->collider->size.x) + 2),
			(float) ((int) (abs(game_cam.collider->size.y - parallax->collider->size.y) / parallax->collider->size.y) + 2)
		)
	);

	//Draw the parallax sprite in a square.
	for (i = -1; i < itersvec.x; i++)
	{
		for (j = -1; j < itersvec.y; j++)
		{

			gf2d_sprite_draw_image(
				parallax->sprite,
				vector2d(
					(i * parallax->collider->size.x) + parallax->collider->worldpos.x - (game_cam.pos.x * parallax->drag),
					(j * parallax->collider->size.y) + parallax->collider->worldpos.y - (game_cam.pos.y * parallax->drag)
				));
		}
	}
}

Bool camera_inview(Collider_Circle* circle, Collider_Box* box)
{
	Bool created_new_box = 0;
	Collider_Box* other_box;

	if (&game_cam == NULL)
	{
		slog("Camera has not been created yet"); return 0;
	}

	if ((circle == NULL) && (box == NULL))
	{
		slog("No valid collider given to check against camera"); return 0;
	}

	//If the input collider is a circle, create a temporary box collider to represent it
	if (circle != NULL)
	{
		created_new_box = 1;

		other_box = box_new();

		vector2d_copy(
			other_box->worldpos, 
			vector2d(
				circle->worldpos.x - circle->radius, 
				circle->worldpos.y - circle->radius
			)
		);

		vector2d_copy(
			other_box->size,
			vector2d(
				circle->radius*2,
				circle->radius*2
			)
		);
	}

	//Else, set the temp box collider to the input box collider.
	else { other_box = box; }

	return box_detectCollision(game_cam.collider, other_box);

}

void camera_move(Vector2D movement_dir)
{
	Vector2D temp;
	Vector2D movement;

	vector2d_copy(movement, game_cam.movementspeed);

	//vector2d_normalize(&movement_dir);

	movement.x = movement.x * movement_dir.x;
	movement.y = movement.y * movement_dir.y;

	vector2d_add(
		temp,
		game_cam.pos, 
		movement
	);

	vector2d_copy(game_cam.pos, temp);

	vector2d_copy(game_cam.collider->worldpos, temp);
}

void camera_free()
{
	box_free(game_cam.collider);
}

void parallax_free(Parallax_Layer* parallax)
{
	box_free(parallax->collider);
	gf2d_sprite_free(parallax->sprite);
}