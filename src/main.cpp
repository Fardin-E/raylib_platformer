#include "header.h"


int main() {

    InitWindow(screenWidth, screenHeight, "Raylib platformer");
    SetTargetFPS(60);

    // Initialize obstacles
    std::vector<Obstacle> envObstacles(4);
    envObstacles[0].Init({ 100, 100, 400, 30 }, { 0, 0 }, 0.0f, GRAY);
    envObstacles[1].Init({ 500, 240, 600, 30 }, { 0, 0 }, 0.0f, GRAY);
    envObstacles[2].Init({ 300, 400, 1200, 30 }, { 0, 0 }, 0.0f, GRAY);
    envObstacles[3].Init({ 200, 600, 600, 30 }, { 0, 0 }, 0.0f, GRAY);

    // Initialize player
    Player player;
    player.Init({ 400, 300 }, 20.0f, BLUE);

    Camera2D camera = { 0 };
    camera.target = player.center;
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose()) {
        // Update each obstacle (if needed)
        for (auto& obstacle : envObstacles) {
            obstacle.Update(); // Call each obstacle's Update method if needed
        }

        // Update player with the list of obstacles
        player.Update(envObstacles);

        camera.target = player.center;

        // Clear the screen
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        // Draw each obstacle
        for (const auto& obstacle : envObstacles) {
            obstacle.Draw();
        }

        // Draw player
        player.Draw();


        EndMode2D();

        EndDrawing();
    }

    CloseWindow(); // Close the window and OpenGL context

    return 0;
}