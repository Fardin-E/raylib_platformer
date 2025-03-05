#pragma once

#include "environment.h"
#include <cmath>
#include "raylib.h"
#include <vector>
#include <algorithm>

// Physics Constants
constexpr float MAX_SPEED = 500.0f;
constexpr float SPEED = 1200.0f;
constexpr float AIR_SPEED = 600.0f;
constexpr float GRAVITY = 800.0f;
constexpr float JUMP_SPEED = 400.0f;
constexpr float AIR_DRAG = 0.98f;
constexpr float GROUND_FRICTION = 0.85f;

constexpr float POS_LOOK_AHEAD_FACTOR = 0.02f;

constexpr float NEG_LOOK_AHEAD_FACTOR = 0.02f;

// Animation Types
enum class AnimationType
{
    ANIM_IDLE,
    ANIM_RUN,
    ANIM_JUMP
};

// Player Direction
enum class PlayerDirection
{
    RIGHT,
    LEFT
};

// Player State
enum class PlayerState
{
    GROUNDED,
    FREE_FALLING,
    JUMPING,
    MOVING
};

// Sprite Animation Class
class SpriteAnimation
{
    public:
    Texture2D *atlas;
    int framesPerSecond;
    float timeStarted;
    std::vector<Rectangle> rectangles;
    AnimationType type;

    SpriteAnimation(Texture2D *atlas, int fps, std::vector<Rectangle> rects, AnimationType type);

    Rectangle GetCurrentFrame() const;
};

// Player Class
class Player
{
    public:
    Rectangle shape;
    Vector2 velocity;
    Camera2D camera;
    PlayerState state;
    PlayerDirection direction;
    PlayerDirection previousDirection;
    std::vector<SpriteAnimation> animationArray;
    AnimationType currentAnimation;

    Player(Rectangle shape, Vector2 velocity, Camera2D camera, PlayerState state, PlayerDirection direction,
        std::vector<SpriteAnimation> animations);

    void Draw(float rotation, float dt, Vector2 origin, Color tint) const;
    void Move(float dt, float acceleration, float max_speed);
    void UpdateCollisionAndState(Environment *environment, float dt);
};
