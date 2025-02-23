#include "header.h"


#define MAX_BUILDINGS 2

Texture2D _texture;
SpriteAnimation _animation;
Player _player;
Environment _environment;


int main(void)
{

	const int screenWidth = 800;
	const int screenHeight = 800;

	InitWindow(screenWidth, screenHeight, "Game");

	SetTraceLogLevel(LOG_WARNING);

	Camera2D camera = { 0 };
	camera.target = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	Rectangle playerShape = { 200.0f, 200.0f, 40.0f, 40.0f };
	Vector2 velocity = { 0.0f, 0.0f };
	Vector2 origin = { 0 };


	Rectangle floor = { 0.0f, 500.0f, 800.0f, 100.0f };

	Rectangle block1 = { 100.0f, 350.0f, 200.0f, 70.0f };

	Rectangle buildings[MAX_BUILDINGS] = {
		floor,
		block1,
	};

	_texture = LoadTexture("assets/mario_spritesheet.png");
	_animation = CreateSpriteAnimation(_texture, 1, (Rectangle[]) {
		(Rectangle){ 36, 1, 34, 26 },
	}, 1);

	_player = CreatePlayer(playerShape, velocity, camera, FREE_FALLING, _animation);
	_environment = CreateEnvironment(buildings, MAX_BUILDINGS);

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		// Update
		float dt = GetFrameTime();

		UpdatePlayerCollisionAndState(&_player, &_environment, dt);

		// Draw
		BeginDrawing();

			ClearBackground(RAYWHITE);

			BeginMode2D(camera);

			DrawPlayer(_player, _animation, 0.0f, dt, origin, RED);

			Init_and_draw_floor(&_environment, BLACK);

			drawBlock(&_environment, (Vector2){ 0.0f, 0.0f }, 0.0f, GREEN);


			EndMode2D();

		EndDrawing();

	}

	DisposeSpriteAnimation(_animation);
	CloseWindow();

	return 0;
}