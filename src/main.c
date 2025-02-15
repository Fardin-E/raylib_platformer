#include "header.h"


// TODO: Need to draw background for a platformer

#define MAX_BUILDINGS 10

Player _player;
Environment _environment;


int main(void)
{

	const int screenWidth = 800;
	const int screenHeight = 800;

	InitWindow(screenWidth, screenHeight, "Game");

	Camera2D camera = { 0 };
	camera.target = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
	camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	Rectangle playerShape = { 200.0f, 200.0f, 40.0f, 40.0f };
	Vector2 origin = { _player.shape.x / 2, _player.shape.y / 2 };
	Vector2 velocity = { 5.0f, 5.0f };


	Rectangle floor = { 0.0f, 500.0f, 800.0f, 100.0f };

	Rectangle buildings[MAX_BUILDINGS] = {
		floor,
	};

	_player = CreatePlayer(playerShape, origin, velocity, camera);
	_environment = CreateEnvironment(buildings, MAX_BUILDINGS);

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		// Update

		// Draw
		BeginDrawing();

			ClearBackground(RAYWHITE);

			BeginMode2D(camera);

			DrawPlayer(_player, 0.0f, RED);

			Init_and_draw_floor(&_environment, floor);


			EndMode2D();

		EndDrawing();

	}

	CloseWindow();

	return 0;
}