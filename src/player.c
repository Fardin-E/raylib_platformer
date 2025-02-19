#include "player.h"

Player CreatePlayer(Rectangle shape, Vector2 velocity, Camera2D camera)
{
	Player player = 
	{
		.shape = shape,
		.velocity = velocity,
		.camera = camera,
        .collision = NO_COLLISION,
		.useGravity = true,
	};

	return player;
}


void DrawPlayer(Player player, float rotation, Color tint)
{
    DrawRectanglePro(player.shape, (Vector2) { player.shape.width / 2, player.shape.height / 2 }, 
        rotation, tint);
}

CollisionType CheckCollision(Player *player, Environment *environment)
{

}

void UpdatePlayer(Player *player, Environment *environment, float dt)
{
    // Use predicted position for x and y axis to implement both horizontal and vertical collision
    Rectangle predictedPos = {  player->shape.x + player->velocity.x * dt,
                                player->shape.y + player->velocity.y * dt,
                                player->shape.width, player->shape.height };

    // Apply gravity to velocity if enabled
    if (player->useGravity)
    {
        player->velocity.y += GRAVITY * dt;
    }

    // Reset horizontal velocity when no input
    if (!IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT))
    {
        player->velocity.x = 0.0f;
    }

    // Horizontal movement
    if (IsKeyDown(KEY_RIGHT) && player->velocity.x < MAX_SPEED)
    {
        player->velocity.x -= SPEED;
    }
    if (IsKeyDown(KEY_LEFT) && player->velocity.x > -MAX_SPEED)
    {
        player->velocity.x += SPEED;
    }

    // Update the player's position based on velocity
    player->shape.x += player->velocity.x * dt;
    player->shape.y += player->velocity.y * dt;

    for (int i = 0; i < environment->blockNum; i++)
    {
        if (CheckCollisionRecs(predictedPos, environment->blocks[i]))
        {
            TraceLog(LOG_WARNING, "Collided");
        }
    }

    // Jumping (when touching ground)
    if (IsKeyPressed(KEY_SPACE) && !player->useGravity)
    {

    }
}