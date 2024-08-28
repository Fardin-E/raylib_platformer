#include "header.h"


int main() {

    InitWindow(screenWidth, screenHeight, "Raylib platformer");
    SetTargetFPS(60);

    // Initialize obstacles
    std::vector<Obstacle> envObstacles(4);
    envObstacles[0].Init({ 100, 100, 400, 20 }, { 0, 0 }, 0.0f, GRAY);
    envObstacles[1].Init({ 500, 240, 600, 20 }, { 0, 0 }, 0.0f, GRAY);
    envObstacles[2].Init({ 300, 400, 700, 20 }, { 0, 0 }, 0.0f, GRAY);
    envObstacles[3].Init({ 200, 600, 600, 20 }, { 0, 0 }, 0.0f, GRAY);

    // Initialize player
    Player player;
    player.Init({ 400, 300 }, 20.0f, BLUE);

    while (!WindowShouldClose()) {
        // Update each obstacle (if needed)
        for (auto& obstacle : envObstacles) {
            obstacle.Update(); // Call each obstacle's Update method if needed
        }

        // Update player with the list of obstacles
        player.Update(envObstacles);

        // Clear the screen
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw each obstacle
        for (const auto& obstacle : envObstacles) {
            obstacle.Draw();
        }

        // Draw player
        player.Draw();

        EndDrawing();
    }

    CloseWindow(); // Close the window and OpenGL context

    return 0;
}