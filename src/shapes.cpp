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

    onGround = false;

    if (isFloating && !onGround) {
        // Apply gravity
        speed += G; 
    }

    // Update vertical position
    center.y += speed;
    

    for (const auto& obs : obstacles) {
        if (CheckCollisionCircleRec(center, radius, obs.envObjects)) {
            // Calculate overlap in X and Y directions
            float overlapX = std::min(center.x + radius - obs.envObjects.x, obs.envObjects.x + obs.envObjects.width - center.x - radius);
            float overlapY = std::min(center.y + radius - obs.envObjects.y, obs.envObjects.y + obs.envObjects.height - center.y - radius);

            if (overlapX < overlapY) {
                // Horizontal collision logic (X-axis)
                if (center.x + radius >= obs.envObjects.x && center.x <= obs.envObjects.x + obs.envObjects.width / 2) {
                    // Collide from the left
                    center.x = obs.envObjects.x - radius;
                }
                else if (center.x - radius <= obs.envObjects.x + obs.envObjects.width && center.x >= obs.envObjects.x + obs.envObjects.width / 2) {
                    // Collide from the right
                    center.x = obs.envObjects.x + obs.envObjects.width + radius;
                }
            }
            else {
                // Vertical collision logic (Y-axis)
                if (center.y + radius >= obs.envObjects.y && center.y <= obs.envObjects.y + obs.envObjects.height / 2) {
                    // Landing on top of the obstacle
                    center.y = obs.envObjects.y - radius;
                    speed = 0;  // Stop downward motion
                    onGround = true;
                    isFloating = false;
                }
                else if (center.y - radius <= obs.envObjects.y + obs.envObjects.height && center.y > obs.envObjects.y + obs.envObjects.height / 2) {
                    // Collide from below
                    center.y = obs.envObjects.y + obs.envObjects.height + radius;
                    speed = 0;  // Stop upward motion
                }
            }
        }
        else {
            isFloating = true;
        }
    }

    // Horizontal movement
    if (IsKeyDown(KEY_RIGHT)) {
        center.x += 5.0f;
    }
    if (IsKeyDown(KEY_LEFT)) {
        center.x -= 5.0f;
    }

    // Debugging mode
    /*if (IsKeyDown(KEY_DOWN)) {
        center.y += 5.0f;
    }
    if (IsKeyDown(KEY_UP)) {
        center.y -= 5.0f;
    }*/

    // Jumping logic
    if (IsKeyPressed(KEY_SPACE)) {
        jumpRequested = true;
    }

    if (jumpRequested && onGround) {
        speed = jumpVelocity;
        isFloating = true;
    }
    jumpRequested = false;  // Clear jump request after jumping
}

void PolyObj::Init(Vector2 initCenter, int initSides, float initRadius, float initRotation,
    Color initColor) {

    center = initCenter;
    sides = initSides;
    radius = initRadius;
    rotation = initRotation;
    color = initColor;
}

void PolyObj::Draw() {
    DrawPoly(center, sides, radius, rotation, color);
}

void PolyObj::Update() {
    
}