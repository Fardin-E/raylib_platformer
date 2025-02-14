#include "header.h"


// TODO: Need to draw background for a platformer

int main(void)
{

	const int screenWidth = 800;
	const int screenHeight = 800;

	InitWindow(screenWidth, screenHeight, "Game");

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		// Update

		// Draw
		BeginDrawing();

			ClearBackground(RAYWHITE);

		EndDrawing();

	}

	CloseWindow();

	return 0;
}