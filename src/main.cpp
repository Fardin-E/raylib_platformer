#include "header.h"

global_variable int screenWidth = 1200;
global_variable int screenHeight = 800;

global_variable float jumpVelocity = -15.0f;
global_variable const float G = 0.5f;

struct platform
{
	Rectangle rect;
	Color color;
};

struct Environment
{
	platform* envObjects;
	int numObjects;

	Environment(int num)
	{
		numObjects = num;
		envObjects = new platform[numObjects];
	}

	~Environment()
	{
		delete[] envObjects;
	}

	void Init()
	{
		envObjects[0] = { {100, 100, 400, 20}, DARKBLUE };
		envObjects[1] = { {500, 240, 600, 20}, GREEN };
		envObjects[2] = { {300, 400, 700, 20}, GRAY };
	}

	void Draw()
	{
		for (int i = 0; i < numObjects; i++)
		{
			DrawRectangleRec(envObjects[i].rect, envObjects[i].color);
		}
	}

};


struct Player
{
	Vector2 position;
	float width, height;
	float speed = 0;
	Color color;
	bool isFloating = true;
	bool onGround = false;
	bool jumpRequested = false;

	void Draw()
	{
		DrawRectangle(position.x, position.y, width, height, color);
	}

	void Update(Environment& env)
	{
		if (isFloating)
		{
			speed += G; // Apply gravity
		}

		// Update position
		position.y += speed;

		onGround = false;

		for (int i = 0; i < env.numObjects; i++)
		{
			platform& envPlatform = env.envObjects[i];

			if (CheckCollisionRecs(
				{ position.x, position.y, width, height },
				envPlatform.rect))
			{
				if (speed > 0) // Only stop falling if moving down
				{
					position.y = envPlatform.rect.y - height; // Set player on top of the platform
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

	Environment environmentObjects(3);
	environmentObjects.Init();
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

		environmentObjects.Draw();
		player.Draw();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}