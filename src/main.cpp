#include "header.h"

global_variable int screenWidth = 1200; // Constants do not take more memory than necessary
global_variable int screenHeight = 800;

int main() {

    InitWindow(screenWidth, screenHeight, "Raylib platformer");
    SetTargetFPS(60);
    
    int buffer = 5;

    // Initialize obstacles
    std::vector<Obstacle> envObstacles(4);
    envObstacles[0].Init({ 100, 700, 200, 500 }, { 0, 0 }, 0.0f, GRAY);
    envObstacles[1].Init({ 400, 700, 200, 500 }, { 0, 0 }, 0.0f, GRAY);
    envObstacles[2].Init({ 650, 700, 200, 500 }, { 0, 0 }, 0.0f, GRAY);
    envObstacles[3].Init({ 900, 700, 200, 500 }, { 0, 0 }, 0.0f, GRAY);


    // Initialize player
    Player player;
    player.Init({ 400, 300 }, 20.0f, BLUE);

    // Initalize Camera
    Camera2D camera = { 0 };
    camera.target = player.center;
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Initialize a polygon
    PolyObj polyO;
    polyO.Init({ 500, 500 }, 6, 50.0f, 0.0f, GREEN);

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

        polyO.Draw();

        EndMode2D();

        EndDrawing();
    }

    CloseWindow(); // Close the window and OpenGL context

    return 0;
}