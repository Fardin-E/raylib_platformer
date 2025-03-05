#include "player.h"
#include "raylib.h"

// SpriteAnimation Constructor
SpriteAnimation::SpriteAnimation(Texture2D *atlas, int fps, std::vector<Rectangle> rects, AnimationType type)
    : atlas(atlas), framesPerSecond(fps), timeStarted(GetTime()), rectangles(std::move(rects)), type(type) {
}

Rectangle SpriteAnimation::GetCurrentFrame() const
{
    if (rectangles.empty()) return { 0, 0, 0, 0 };
    int index = static_cast<int>((GetTime() - timeStarted) * framesPerSecond) % rectangles.size();
    return rectangles[index];
}

// Player Constructor
Player::Player(Rectangle shape, Vector2 velocity, Camera2D camera, PlayerState state, PlayerDirection direction,
    std::vector<SpriteAnimation> animations)
    : shape(shape), velocity(velocity), camera(camera), state(state), direction(direction),
    previousDirection(direction), animationArray(std::move(animations)), currentAnimation(AnimationType::ANIM_IDLE) {
}

// Draw Player
void Player::Draw(float rotation, float dt, Vector2 origin, Color tint) const
{
    if (animationArray.empty()) return;

    const SpriteAnimation &anim = animationArray[static_cast<int>(currentAnimation)];
    if (anim.rectangles.empty()) return;

    Rectangle source = anim.GetCurrentFrame();
    source.width = (direction == PlayerDirection::LEFT) ? -std::abs(source.width) : std::abs(source.width);

    DrawTexturePro(*anim.atlas, source, shape, origin, rotation, tint);
}

// Move Player
void Player::Move(float dt, float acceleration, float max_speed)
{
    bool isInAir = (state == PlayerState::JUMPING || state == PlayerState::FREE_FALLING);
    float frictionValue = isInAir ? AIR_DRAG : GROUND_FRICTION;

    if (IsKeyDown(KEY_RIGHT))
    {
        velocity.x += acceleration * dt;
        velocity.x = std::min(velocity.x, max_speed);
        direction = PlayerDirection::RIGHT;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        velocity.x -= acceleration * dt;
        velocity.x = std::max(velocity.x, -max_speed);
        direction = PlayerDirection::LEFT;
    }
    else
    {
        velocity.x *= frictionValue;
        if (std::abs(velocity.x) < 0.1f) velocity.x = 0.0f;
    }
}

// Update Collision and State
void Player::UpdateCollisionAndState(Environment *environment, float dt)
{
    if (state != PlayerState::GROUNDED)
    {
        velocity.y += GRAVITY * dt;
        Move(dt, AIR_SPEED, MAX_SPEED);
    }
    else
    {
        Move(dt, SPEED, MAX_SPEED);
    }

    float newX = shape.x + velocity.x * dt;
    float newY = shape.y + velocity.y * dt;
    bool isOnGround = false;

    // Check for ground collision
    Rectangle groundCheckRect = { shape.x, shape.y + shape.height, shape.width, 2.0f };
    for (const auto &block : environment->blocks)
    {
        if (CheckCollisionRecs(groundCheckRect, block))
        {
            isOnGround = true;
            break;
        }
    }

    // Collision Handling
    for (const auto &block : environment->blocks)
    {
        Rectangle predictedPosX = { newX, shape.y, shape.width, shape.height };
        Rectangle predictedPosY = { shape.x, newY, shape.width, shape.height };

        if (CheckCollisionRecs(predictedPosX, block))
        {
            newX = velocity.x > 0 ? block.x - shape.width : block.x + block.width;
            velocity.x = 0;
        }
        if (CheckCollisionRecs(predictedPosY, block))
        {
            if (velocity.y > 0)
            {
                newY = block.y - shape.height;
                isOnGround = true;
            }
            else
            {
                newY = block.y + block.height;
            }
            velocity.y = 0;
        }
    }

    shape.x = newX;
    shape.y = newY;

    static int stateChangeDelay = 0;
    constexpr int STATE_CHANGE_THRESHOLD = 5;

    // State Transitions
    if (isOnGround)
    {
        if (state == PlayerState::JUMPING || state == PlayerState::FREE_FALLING)
        {
            stateChangeDelay = STATE_CHANGE_THRESHOLD;
        }

        if (IsKeyDown(KEY_SPACE) && state != PlayerState::JUMPING)
        {
            velocity.y = -JUMP_SPEED;
            state = PlayerState::JUMPING;
            stateChangeDelay = 0;
        }
        else if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) && std::abs(velocity.x) > 0.1f)
        {
            state = PlayerState::MOVING;
            stateChangeDelay = 0;
        }
        else
        {
            state = PlayerState::GROUNDED;
        }
    }
    else if (stateChangeDelay > 0)
    {
        stateChangeDelay--;
    }
    else
    {
        state = (velocity.y < 0) ? PlayerState::JUMPING : PlayerState::FREE_FALLING;
    }

    // Animation Handling
    switch (state)
    {
        case PlayerState::GROUNDED:
            currentAnimation = AnimationType::ANIM_IDLE;
            break;
        case PlayerState::MOVING:
            currentAnimation = AnimationType::ANIM_RUN;
            break;
        case PlayerState::JUMPING:
        case PlayerState::FREE_FALLING:
            currentAnimation = AnimationType::ANIM_JUMP;
            break;
        default:
            currentAnimation = AnimationType::ANIM_IDLE;
            break;
    }
}
