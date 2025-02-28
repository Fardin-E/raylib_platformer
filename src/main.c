#include "header.h"

#define MAX_BUILDINGS 2

Texture2D _texture;
SpriteAnimation _idle_animation;
SpriteAnimation _run_animation;
SpriteAnimation _jump_animation;


Player *_player;       // pointer to a dynamically allocated Player
Environment _environment;

// Camera smoothing factor (1.0 = instant, lower values = smoother)
#define CAMERA_SMOOTHNESS 0.1f

void PlayerCamera(Camera2D *camera, Player *player, float dt, int screenWidth, int screenHeight)
{
    // Calculate the player's center
    float baseTargetX = player->shape.x + player->shape.width / 2;
    float baseTargetY = player->shape.y + player->shape.height / 2;

    // Smoothly interpolate the camera target towards the player's center
    camera->target.x += (baseTargetX - camera->target.x) * CAMERA_SMOOTHNESS;
    camera->target.y += (baseTargetY - camera->target.y) * CAMERA_SMOOTHNESS;

    // Calculate look-ahead offset based on player velocity
    float lookAheadOffset = 0.0f;
    if (player->velocity.x > 0) {
        lookAheadOffset = player->velocity.x * POS_LOOK_AHEAD_FACTOR;
    }
    else if (player->velocity.x < 0) {
        lookAheadOffset = player->velocity.x * NEG_LOOK_AHEAD_FACTOR;
    }

    // Add look-ahead offset AFTER smoothing
    camera->target.x += lookAheadOffset;

    // Now clamp the camera target.
    // Adjust these values according to your level design.
    // For example, if your level starts at x=0, set minX accordingly.
    float minX = player->shape.x;          // allow camera to move fully to the left edge
    float maxX = MAX_BUILDINGS * 800;      // level width
    float minY = screenHeight / 2 / camera->zoom;
    float maxY = 1000 - minY;

    camera->target.x = Clamp(camera->target.x, minX, maxX);
    camera->target.y = Clamp(camera->target.y, minY, maxY);
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Game");

    SetTraceLogLevel(LOG_WARNING);

    // Player properties
    Rectangle playerShape = { 200.0f, 200.0f, 25.0f, 40.0f };
    Vector2 velocity = { 0.0f, 0.0f };
    Vector2 origin = { 0.0f, 0.0f };

    // Setup Camera
    Camera2D camera = { 0 };
    camera.target = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Environment blocks
    Rectangle floor = { 0.0f, 500.0f, 800.0f, 100.0f };
    Rectangle block1 = { 100.0f, 350.0f, 200.0f, 70.0f };

    Rectangle buildings[MAX_BUILDINGS] = { floor, block1 };

    // Load texture and create sprite animation
    _texture = LoadTexture("assets/mario_spritesheet.png");

    Rectangle idle[] = { (Rectangle) { 47, 10, 12, 15 } };
    _idle_animation = CreateSpriteAnimation(_texture, 1, idle, 1, ANIM_IDLE);


    Rectangle run[] = { (Rectangle){ 80, 9, 15, 16 }, 
                        (Rectangle){ 117, 10, 12, 15 },
                        (Rectangle){ 150, 9, 15, 16 }, };
    _run_animation = CreateSpriteAnimation(_texture, 9, run, 3, ANIM_RUN);


    Rectangle jump[] = { (Rectangle){ 395, 9, 15, 16 } };
    _jump_animation = CreateSpriteAnimation(_texture, 1, jump, 1, ANIM_JUMP);


    // Build the animation array for the player (even if just one element)
    SpriteAnimation playerAnimations[] = { _idle_animation, _run_animation, _jump_animation };

    // Create the player dynamically
    _player = CreatePlayer(playerShape, velocity, camera, FREE_FALLING, RIGHT, playerAnimations, 3);
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

        // Update camera to follow player
        PlayerCamera(&camera, _player, dt, screenWidth, screenHeight);

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
