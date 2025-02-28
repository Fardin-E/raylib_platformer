#pragma once

#include "environment.h"
#include <math.h>
#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>

// physics constants
#define MAX_SPEED 500.0f      
#define SPEED 1200.0f        
#define AIR_SPEED 600.0f      
#define GRAVITY 800.0f        
#define JUMP_SPEED 400.0f     
#define AIR_DRAG 0.98f        
#define GROUND_FRICTION 0.85f 

#define POS_LOOK_AHEAD_FACTOR 0.02f

#define NEG_LOOK_AHEAD_FACTOR 0.02f


typedef enum
{
    ANIM_IDLE,
    ANIM_RUNR,
    ANIM_RUNL,
    ANIM_JUMPR,
    ANIM_JUMPL,
} AnimationType;

typedef struct SpriteAnimation
{
    Texture2D atlas;
    int framesPerSecond;
    float timeStarted;
    Rectangle *rectangles;
    int rectanglesLength;
    AnimationType type;
} SpriteAnimation;

typedef enum
{
    GROUNDED = 0,
    FREE_FALLINGL,
    FREE_FALLINGR,
    JUMPING_RIGHT,
    JUMPING_LEFT,
    MOVING_RIGHT,
    MOVING_LEFT,
} PlayerState;

typedef struct Player
{
    Rectangle shape;
    Vector2 velocity;
    Camera2D camera;
    PlayerState state;
    SpriteAnimation *animation_array;
    AnimationType currentAnimation;
    int array_length;
} Player;


// Animation function sig
SpriteAnimation CreateSpriteAnimation(Texture2D atlas, int framesPerSecond, 
    Rectangle rectangles[], int length, AnimationType animeType);
void DisposeSpriteAnimation(SpriteAnimation *animation);

// player function sig
Player *CreatePlayer(Rectangle shape, Vector2 velocity, Camera2D camera,
    PlayerState state, SpriteAnimation animation_array[], int array_length);
void DisposePlayer(Player *player);
void DrawPlayer(Player *player, float rotation, float dt, Vector2 origin, Color tint);
void PlayerMovement(Player *player, float dt, float speed, float max_speed);
void UpdatePlayerCollisionAndState(Player *player, Environment *environment, float dt);
