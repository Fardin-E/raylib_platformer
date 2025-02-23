#include "player.h"

Player CreatePlayer(Rectangle shape, Vector2 velocity, Camera2D camera, PlayerState state)
{
	Player player = 
	{
		.shape = shape,
		.velocity = velocity,
		.camera = camera,
        .state = state,
	};

	return player;
}


void DrawPlayer(Player player, float rotation, Color tint)
{
    DrawRectanglePro(player.shape, (Vector2) { 0, 0 },
        rotation, tint);
}


void UpdatePlayerCollisionAndState(Player *player, Environment *environment, float dt)
{
    // Movement
    if (IsKeyDown(KEY_RIGHT))
    {
        player->velocity.x = fminf(player->velocity.x + SPEED, MAX_SPEED);
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        player->velocity.x = fmaxf(player->velocity.x - SPEED, -MAX_SPEED);
    }
    else
    {
        player->velocity.x = 0.0f;
    }
    // Calculate predicted positions
    float newX = player->shape.x + player->velocity.x * dt;
    float newY = player->shape.y + player->velocity.y * dt;
    bool collisionFound = false;

    // Apply gravity consistently in non-grounded states
    if (player->state != GROUNDED) {
        player->velocity.y += GRAVITY * dt;
    }

    // Check collision against each environment block
    for (int i = 0; i < environment->blockNum; i++)
    {
        Rectangle block = environment->blocks[i];
        Rectangle predictedPosX = { newX, player->shape.y, player->shape.width, player->shape.height };
        Rectangle predictedPosY = { player->shape.x, newY, player->shape.width, player->shape.height };

        // Check horizontal collision
        if (CheckCollisionRecs(predictedPosX, block))
        {
            if (player->velocity.x > 0) {
                newX = block.x - player->shape.width;
            }
            else if (player->velocity.x < 0) {
                newX = block.x + block.width;
            }
            player->velocity.x = 0;
            player->state = HORIZONTAL_COLLISION;
            collisionFound = true;
        }

        // Check vertical collision
        if (CheckCollisionRecs(predictedPosY, block))
        {
            if (player->velocity.y > 0) {
                newY = block.y - player->shape.height;
                player->state = GROUNDED;
            }
            else if (player->velocity.y < 0) {
                newY = block.y + block.height;
                player->state = VERTICAL_COLLISION;
            }
            player->velocity.y = 0;
            collisionFound = true;
            break;
        }
    }

    // Handle state transitions
    if (!collisionFound) {
        if (player->state == GROUNDED) {
            player->state = FREE_FALLING;
        }
    }

    // Update positions
    player->shape.x = newX;
    player->shape.y = newY;

    // State-specific updates
    switch (player->state)
    {
        case GROUNDED:
            if (IsKeyDown(KEY_SPACE)) {
                player->velocity.y = -JUMP_SPEED;
                player->state = JUMPING;
            }
            break;

        case JUMPING:
            // Apply air resistance to x velocity
            player->velocity.x *= AIR_DRAG;

            if (fabsf(player->velocity.x) < 0.1f) {
                player->velocity.x = 0;
            }

            // Transition to falling at peak of jump
            if (player->velocity.y > 0) {
                player->state = FREE_FALLING;
            }
            break;

        case FREE_FALLING:
            // Also apply air resistance while falling
            player->velocity.x *= AIR_DRAG;
            break;

        case HORIZONTAL_COLLISION:
        case VERTICAL_COLLISION:
            // These states are handled during collision detection
            break;
    }
}