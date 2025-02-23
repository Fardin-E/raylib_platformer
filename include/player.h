#pragma once

#include "environment.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>

// Physics constants
#define MAX_SPEED 500.0f
#define SPEED 300.0f          // Increased for more responsive movement
#define GRAVITY 800.0f        // Increased for more natural falling
#define JUMP_SPEED 400.0f     // Increased for higher jumps
#define AIR_DRAG 0.95f        // Added for air control

typedef enum
{
	FREE_FALLING = 0,
	VERTICAL_COLLISION,
	HORIZONTAL_COLLISION,
	JUMPING,
	GROUNDED,
} PlayerState;

typedef struct Player
{
	Rectangle shape;
	Vector2 velocity;
	Camera2D camera;
	PlayerState state;
} Player;

float newX;
float newY;

Rectangle predictedPosX;
Rectangle predictedPosY;


Player CreatePlayer(Rectangle shape, Vector2 velocity, Camera2D camera, PlayerState state);

void DrawPlayer(Player player, float rotation, Color tint);

void UpdatePlayerCollisionAndState(Player *player, Environment *environment, float dt);
