#pragma once

#include "raylib.h"
#include <stdbool.h>

typedef struct Player
{
	Rectangle shape;
	Vector2 startingPosition;
	Vector2 velocity;
	bool useGravity;
	Camera2D camera;
} Player;

Player CreatePlayer(Rectangle shape, Vector2 startingPosition, Vector2 velocity, Camera2D camera);

void DrawPlayer(Player player, float rotation, Color tint);
