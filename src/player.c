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
    // Apply gravity to vertical velocity if enabled
    if (player->useGravity)
    {
        player->velocity.y += GRAVITY * dt;
    }

    // Reset horizontal velocity when no input
    if (!IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT))
    {
        player->velocity.x = 0.0f;
    }

    // Horizontal movement input
    if (IsKeyDown(KEY_RIGHT) && player->velocity.x < MAX_SPEED)
    {
        // Increase velocity to the right
        player->velocity.x += SPEED;
    }
    if (IsKeyDown(KEY_LEFT) && player->velocity.x > -MAX_SPEED)
    {
        // Increase velocity to the left (more negative)
        player->velocity.x -= SPEED;
    }

    // Calculate new predicted positions based on velocity
    float newX = player->shape.x + player->velocity.x * dt;
    float newY = player->shape.y + player->velocity.y * dt;

    // Predictive collision rectangle
    Rectangle predictedPos = { newX, newY, player->shape.width, player->shape.height };

    // Check collision against each environment block
    for (int i = 0; i < environment->blockNum; i++)
    {
        if (CheckCollisionRecs(predictedPos, environment->blocks[i]))
        {
            TraceLog(LOG_WARNING, "Collided with block %d", i);
            newY = player->shape.y;
            player->velocity.y = 0;
            break;
        }
    }

    // Update the player's position with the new values
    player->shape.x = newX;
    player->shape.y = newY;

    // Jumping (example: if space is pressed and the player is on the ground)
    if (IsKeyPressed(KEY_SPACE) && player->velocity.y == 0)
    {
        player->velocity.y = -JUMP_SPEED;  // Ensure JUMP_SPEED is defined appropriately
    }
}
