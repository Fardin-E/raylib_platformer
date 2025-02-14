#include "player.h"

Player CreatePlayer(Rectangle shape, Vector2 startingPosition, Vector2 velocity, 
	Camera2D camera)
{
	Player player = 
	{
		.shape = shape,
		.startingPosition = startingPosition,
		.velocity = velocity,
		.useGravity = true,
		.camera = camera
	};

	return player;
}


void DrawPlayer(Player player, float rotation, Color tint)
{
	DrawRectanglePro(player.shape, player.startingPosition, rotation, tint);
}