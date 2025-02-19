#pragma once

#include "environment.h"
#include "raylib.h"
#include <stdbool.h>

#define MAX_SPEED 300.0f
#define SPEED 20.0f
#define GRAVITY 55.0f
#define JUMP_SPEED 100.0f

typedef struct Player
{
	Rectangle shape;
	Vector2 velocity;
	Camera2D camera;
	int collision;
	bool useGravity;
} Player;

typedef enum CollisionType
{
	NO_COLLISION,
	VERTICAL_COLLISION,
	HORIZONTAL_COLLISION
} CollisionType;

Player CreatePlayer(Rectangle shape, Vector2 velocity, Camera2D camera);

void DrawPlayer(Player player, float rotation, Color tint);

CollisionType CheckCollision(Player *player, Environment *environment);

void UpdatePlayer(Player *player, Environment *environment, float dt);
