#include "header.h"

#define MAX_BUILDINGS 2

Texture2D _texture;
SpriteAnimation _animation;
Player *_player;       // Now a pointer to a dynamically allocated Player
Environment _environment;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Game");

    SetTraceLogLevel(LOG_WARNING);

    // Setup Camera
    Camera2D camera = { 0 };
    camera.target = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Player properties
    Rectangle playerShape = { 200.0f, 200.0f, 40.0f, 40.0f };
    Vector2 velocity = { 0.0f, 0.0f };
    Vector2 origin = { 0.0f, 0.0f };

    // Environment blocks
    Rectangle floor = { 0.0f, 500.0f, 800.0f, 100.0f };
    Rectangle block1 = { 100.0f, 350.0f, 200.0f, 70.0f };

    Rectangle buildings[MAX_BUILDINGS] = { floor, block1 };

    // Load texture and create sprite animation
    _texture = LoadTexture("assets/mario_spritesheet.png");
    _animation = CreateSpriteAnimation(_texture, 1, (Rectangle[]) {
        (Rectangle) { 36, 1, 34, 26 },
    }, 1);

    // Build the animation array for the player (even if just one element)
    SpriteAnimation playerAnimations[1] = { _animation };

    // Create the player dynamically
    _player = CreatePlayer(playerShape, velocity, camera, FREE_FALLING, playerAnimations, 1);
    if (_player == NULL)
    {
        TraceLog(LOG_ERROR, "Failed to create player!");
        CloseWindow();
        return 1;
    }

    // Create environment (assuming CreateEnvironment returns Environment)
    _environment = CreateEnvironment(buildings, MAX_BUILDINGS);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Update game logic
        float dt = GetFrameTime();
        UpdatePlayerCollisionAndState(_player, &_environment, dt);

        // Begin drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);
        // Dereference _player since DrawPlayer takes a Player by value
        DrawPlayer(_player, 0.0f, dt, origin, WHITE);

        Init_and_draw_floor(&_environment, BLACK);
        drawBlock(&_environment, (Vector2) { 0.0f, 0.0f }, 0.0f, GREEN);
        EndMode2D();

        EndDrawing();
    }

    // Free allocated memory
    DisposePlayer(_player);

    CloseWindow();
    return 0;
}
