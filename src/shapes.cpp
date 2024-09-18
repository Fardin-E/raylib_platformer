#include "shapes.h"

void Obstacle::Init(Rectangle initObjects, Vector2 initOrigin,
    float initRotation, Color initColor)
{
    envObjects = initObjects;
    origin = initOrigin;
    rotation = initRotation;
    color = initColor;
}

void Obstacle::Draw() const
{
    DrawRectanglePro(envObjects, origin, rotation, color);
}

void Obstacle::Update()
{
    // Implement individual obstacle update logic here if needed
}

void Player::Init(Vector2 initCenter, float initRadius, Color initColor)
{
    center = initCenter;
    radius = initRadius;
    color = initColor;
}

void Player::Draw()
{
    DrawCircleV(center, radius, color);
}

void Player::Update(const std::vector<Obstacle>& obstacles) {
    if (isFloating) {
        //speed += G; // Apply gravity
    }

    // Update position
    center.y += speed;
    onGround = false;

    for (const auto& obs : obstacles) {
        if (CheckCollisionCircleRec(center, radius, obs.envObjects)) {
            //if (speed > 0) { // Only stop falling if moving down
            //
            //    
            //    center.y = obs.envObjects.y - radius;
            //
            //    if (center.y + radius >= obs.envObjects.y + obs.envObjects.height / 2)
            //    {
            //        speed -= 1;
            //        isFloating = true;
            //    }
            //    else
            //    {
            //        center.y = obs.envObjects.y - radius; // Set player on top of the platform
            //    }
            //}
            //speed = 0; // Stop downward motion
            //onGround = true; // Player is now on a platform

            // Vertical collision logic (Y-axis)

            if (center.y + radius >= obs.envObjects.y && center.y <= obs.envObjects.y + obs.envObjects.height / 2)
            {
                center.y = obs.envObjects.y - radius;
            }
            else if (center.y - radius <= obs.envObjects.y + obs.envObjects.height && center.y > obs.envObjects.y + obs.envObjects.height / 2)
            {
                center.y = obs.envObjects.y + obs.envObjects.height + radius;
            }

            // Horizontal collision logic (X-axis)
            
        }   

    }

    // Horizontal movement
    if (IsKeyDown(KEY_RIGHT)) {
        center.x += 5.0f;
    }
    if (IsKeyDown(KEY_LEFT)) {
        center.x -= 5.0f;
    }

    // testing mode
    if (IsKeyDown(KEY_DOWN)) {
        center.y += 5.0f;
    }
    if (IsKeyDown(KEY_UP)) {
        center.y -= 5.0f;
    }

    // Jumping
    if (IsKeyPressed(KEY_SPACE)) {
        jumpRequested = true;
    }

    if (jumpRequested && onGround) {
        speed = jumpVelocity;
        isFloating = true;
    }
    jumpRequested = false; // Clear jump request after jumping
}
