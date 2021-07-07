#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

#include "collision.h"

typedef struct
{

	Sprite* sprite;
	Collider_Box* collider;

	float drag;

} Parallax_Layer;

typedef struct
{

	Vector2D pos;
	Vector2D size;

	Vector2D movementspeed;

	Collider_Box* collider;

} Camera;

/*
* @brief Get the game's camera
* @return The game's camera struct
*/
Camera* camera_get();

/*
* @brief Initialize the game's camera
* @param pos The camera's position
* @param size The size of the camera
* @param movementspeed How much the camera moves in the x or y direction in a frame
* @return The game's camera struct
*/
Camera* camera_init(Vector2D pos, Vector2D size, Vector2D movementspeed);

/*
* @brief Add parallax sprite to camera
* @param parallax_sprite The sprite that will be a parallax layer above the background
* @return a Parallax_Layer
*/
Parallax_Layer* parallax_init(Sprite* parallax_sprite, Vector2D pos, float drag);

/*
* @brief Draw the parallax sprite belonging to the camera in a loop
*/
void camera_parallax_draw(Parallax_Layer* parallax);

/*
* @brief Check if an object is in the camera's view (requires one of the two optional params)
* @param circle (Optional) circle collider of the object
* @param box (Optional) box collider of the object
* @return 1 if in view, 0 if not
*/
Bool camera_inview(Collider_Circle* circle, Collider_Box* box);

/*
* @brief Move the camera
* @param movement_dir A vector indicating in which direction the camera is moving, with -1, 0 or 1 the values
*/
void camera_move(Vector2D movement_dir);

/*
* @brief Move the camera
* @param movement_dir A vector indicating in which direction the camera is moving, with -1, 0 or 1 the values
*/
void camera_drag(Vector2D movement_dir);

/*
* @brief Free the game's camera
*/
void camera_free();

/*
* @brief Free a parallax layer
*/
void parallax_free(Parallax_Layer* parallax);

#endif