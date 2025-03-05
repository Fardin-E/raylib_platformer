#include "header.h"
#include <memory>

#define MAX_BUILDINGS 2

Texture2D _texture;
std::unique_ptr<Player> _player;  // Unique pointer for player
std::unique_ptr<Environment> _environment;  // Unique pointer for environment

// Use unique_ptr for the animation objects as well
std::unique_ptr<SpriteAnimation> _idle_animation;
std::unique_ptr<SpriteAnimation> _run_animation;
std::unique_ptr<SpriteAnimation> _jump_animation;

constexpr float CAMERA_SMOOTHNESS = 0.1f;

void PlayerCamera(Camera2D &camera, const Player &player, float dt, int screenWidth, int screenHeight)
{
    float baseTargetX = player.shape.x + player.shape.width / 2;
    float baseTargetY = player.shape.y + player.shape.height / 2;

    camera.target.x += (baseTargetX - camera.target.x) * CAMERA_SMOOTHNESS;
    camera.target.y += (baseTargetY - camera.target.y) * CAMERA_SMOOTHNESS;

    float lookAheadOffset = (player.velocity.x > 0) ? player.velocity.x * POS_LOOK_AHEAD_FACTOR
        : (player.velocity.x < 0) ? player.velocity.x * NEG_LOOK_AHEAD_FACTOR
        : 0.0f;

    camera.target.x += lookAheadOffset;

    float minX = player.shape.x;
    float maxX = MAX_BUILDINGS * 800;
    float minY = screenHeight / 2 / camera.zoom;
    float maxY = 1000 - minY;

    camera.target.x = Clamp(camera.target.x, minX, maxX);
    camera.target.y = Clamp(camera.target.y, minY, maxY);
}

void InitAnimations()
{
    _texture = LoadTexture("assets/mario_spritesheet.png");

    std::vector<Rectangle> idleFrames = { {47, 10, 12, 15} };
    _idle_animation = std::make_unique<SpriteAnimation>(&_texture, 1, idleFrames, AnimationType::ANIM_IDLE);

    std::vector<Rectangle> runFrames = { {80, 9, 15, 16}, {117, 10, 12, 15}, {150, 9, 15, 16} };
    _run_animation = std::make_unique<SpriteAnimation>(&_texture, 9, runFrames, AnimationType::ANIM_RUN);

    std::vector<Rectangle> jumpFrames = { {395, 9, 15, 16} };
    _jump_animation = std::make_unique<SpriteAnimation>(&_texture, 1, jumpFrames, AnimationType::ANIM_JUMP);
}

int main()
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Game");
    SetTraceLogLevel(LOG_WARNING);

    Rectangle playerShape = { 200.0f, 200.0f, 25.0f, 40.0f };
    Vector2 velocity = { 0.0f, 0.0f };
    Vector2 origin = { 0.0f, 0.0f };

    Camera2D camera = { {screenWidth / 2.0f, screenHeight / 2.0f},
                        {screenWidth / 2.0f, screenHeight / 2.0f},
                        0.0f, 1.0f };

    Rectangle floor = { 0.0f, 500.0f, 800.0f, 100.0f };
    Rectangle block1 = { 100.0f, 350.0f, 200.0f, 70.0f };

    std::vector<Rectangle> buildings = { floor, block1 };

    InitAnimations();

    // Now initializing the environment dynamically with buildings
    _environment = std::make_unique<Environment>(buildings);

    std::vector<SpriteAnimation> playerAnimations = { *_idle_animation, *_run_animation, *_jump_animation };

    _player = std::make_unique<Player>(playerShape, velocity, camera, PlayerState::FREE_FALLING, PlayerDirection::RIGHT, playerAnimations);
    if (!_player)
    {
        TraceLog(LOG_ERROR, "Failed to create player!");
        CloseWindow();
        return 1;
    }

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        _player->UpdateCollisionAndState(_environment.get(), dt); // Use .get() to pass the raw pointer to functions
        PlayerCamera(camera, *_player, dt, screenWidth, screenHeight);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);

        _player->Draw(0.0f, dt, origin, WHITE);
        _environment->InitAndDrawFloor(BLACK); // Use -> for accessing methods of unique_ptr
        _environment->DrawBlocks({ 0.0f, 0.0f }, 0.0f, GREEN); // Same here

        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
