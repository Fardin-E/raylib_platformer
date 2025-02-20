#pragma once

#include "environment.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>

#define MAX_SPEED 500.0f
#define SPEED 100.0f
#define GRAVITY 100.0f
#define JUMP_SPEED 200.0f

typedef enum
{
	DEFAULT_COLLISION = 0,
	VERTICAL_COLLISION,
	HORIZONTAL_COLLISION,
	FREE_FALLING,
	GROUNDED,
} PlayerState;

typedef struct Player
{
	Rectangle shape;
	Vector2 velocity;
	Camera2D camera;
	PlayerState state;
	bool useGravity;
} Player;



Player CreatePlayer(Rectangle shape, Vector2 velocity, Camera2D camera, PlayerState state);

void DrawPlayer(Player player, float rotation, Color tint);

PlayerState state(Player *player, Environment *environment);

void UpdatePlayer(Player *player, Environment *environment, float dt);
