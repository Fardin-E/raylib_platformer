#pragma once

#include "environment.h"
#include <math.h>
#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>

// Physics constants
#define MAX_SPEED 500.0f
#define SPEED 300.0f
#define GRAVITY 800.0f
#define JUMP_SPEED 400.0f
#define AIR_DRAG 0.95f

typedef enum
{
    FREE_FALLING = 0,
    VERTICAL_COLLISION,
    HORIZONTAL_COLLISION,
    JUMPING,
    GROUNDED,
} PlayerState;

typedef struct SpriteAnimation
{
    Texture2D atlas;
    int framesPerSecond;
    float timeStarted;
    Rectangle *rectangles;
    int rectanglesLength;
} SpriteAnimation;

typedef struct Player
{
    Rectangle shape;
    Vector2 velocity;
    Camera2D camera;
    PlayerState state;
    SpriteAnimation animation;
} Player;


// Function declarations
SpriteAnimation CreateSpriteAnimation(Texture2D atlas, int framesPerSecond, Rectangle rectangles[], int length);
void DisposeSpriteAnimation(SpriteAnimation animation);


Player CreatePlayer(Rectangle shape, Vector2 velocity, Camera2D camera, PlayerState state, SpriteAnimation animation);
void DrawPlayer(Player player, SpriteAnimation animation, float rotation, float dt, Vector2 origin, Color tint);
void UpdatePlayerCollisionAndState(Player *player, Environment *environment, float dt);
