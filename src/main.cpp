#include "header.h"

const int screenWidth = 1200; // Constants do not take more memory than necessary
const int screenHeight = 800;

const float jumpVelocity = -15.0f;
const float G = 0.5f;


struct Environment
{
    Rectangle envObjects[4]; // Stack allocation with a fixed number of platforms

    Environment()
    {
        Init();
    }

    void Init()
    {
        envObjects[0] = {100, 100, 400, 20};
        envObjects[1] = {500, 240, 600, 20};
        envObjects[2] = {300, 400, 700, 20};
        envObjects[3] = {200, 600, 600, 20};
    }

    void Drawflat() const
    {
        for (int i = 0; i < 3; i++)
        {
            DrawRectangleRec(envObjects[i], GREEN);
        }
    }
    void Drawpro() const
    {
        DrawRectanglePro(envObjects[3], Vector2{ 0, 0 }, 30.0f, GRAY);
    }
};

struct Player
{
    Vector2 position{};
    float width{}, height{};
    float speed = 0;
    Color color{};
    bool isFloating = true;
    bool onGround = false;
    bool jumpRequested = false;

    void Draw() const
    {
        DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(width), static_cast<int>(height), color);
    }

    void Update(const Environment& env)
    {
        if (isFloating)
        {
            speed += G; // Apply gravity
        }

        // Update position
        position.y += speed;
        onGround = false;

        for (const Rectangle& envPlatform : env.envObjects)
        {
            if (CheckCollisionRecs(
                { position.x, position.y, width, height },
                envPlatform))
            {
                if (speed > 0) // Only stop falling if moving down
                {
                    position.y = envPlatform.y - height; // Set player on top of the platform
                    speed = 0; // Stop downward motion
                    onGround = true; // Player is now on a platform
                    break; // No need to check other platforms
                }
            }
        }

        // Horizontal movement
        if (IsKeyDown(KEY_RIGHT))
        {
            position.x += 5.0f;
        }
        if (IsKeyDown(KEY_LEFT))
        {
            position.x -= 5.0f;
        }

        // Jumping
        if (IsKeyPressed(KEY_SPACE))
        {
            jumpRequested = true;
        }

        if (jumpRequested && onGround)
        {
            speed = jumpVelocity;
            isFloating = true;
        }
        jumpRequested = false; // Clear jump request after jumping
    }
};

int main()
{
    InitWindow(screenWidth, screenHeight, "Platformer");
    SetTargetFPS(60);

    Environment environmentObjects;
    Player player;
    player.position = { 400, 280 };
    player.width = 30;
    player.height = 30;
    player.color = RED;

    while (!WindowShouldClose())
    {
        // Update
        player.Update(environmentObjects);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        environmentObjects.Drawflat();
        environmentObjects.Drawpro();
        player.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
