#include "player.h"

Player CreatePlayer(Rectangle shape, Vector2 velocity, Camera2D camera, PlayerState state)
{
	Player player = 
	{
		.shape = shape,
		.velocity = velocity,
		.camera = camera,
        .state = state,
		.useGravity = true,
	};

	return player;
}


void DrawPlayer(Player player, float rotation, Color tint)
{
    DrawRectanglePro(player.shape, (Vector2) { 0, 0 },
        rotation, tint);
}

PlayerState state(Player *player, Environment *environment)
{

}


void UpdatePlayer(Player *player, Environment *environment, float dt)
{
    // Horizontal movement input
    if (IsKeyDown(KEY_RIGHT))
    {
        player->velocity.x = fmin(player->velocity.x + SPEED * dt, MAX_SPEED);
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        player->velocity.x = fmax(player->velocity.x - SPEED * dt, -MAX_SPEED);
    }
    else
    {
        player->velocity.x = 0.0f;
    }

    // Vertical movement (gravity)
    if (player->useGravity)
    {
        player->velocity.y += GRAVITY * dt;
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

    // horizontal collision
    float newX = player->shape.x + player->velocity.x * dt;

    Rectangle predictedPosX = { newX, player->shape.y, player->shape.width, player->shape.height };

    // Check collision against each environment block
    for (int i = 0; i < environment->blockNum; i++)
    {
        if (CheckCollisionRecs(predictedPosX, environment->blocks[i]))
        {
            if (player->velocity.x > 0) {
                newX = environment->blocks[i].x - player->shape.width;
            }
            else if (player->velocity.x < 0) {
                newX = environment->blocks[i].x + environment->blocks[i].width;
            }
            player->velocity.x = 0;
            break;
        }
    }

    player->shape.x = newX;

    bool onGround = false;

    // Vertical collision
    float newY = player->shape.y + player->velocity.y * dt;

    // Predictive collision rectangle
    Rectangle predictedPosY = { player->shape.x, newY, player->shape.width, player->shape.height };

    // Check collision against each environment block
    for (int i = 0; i < environment->blockNum; i++)
    {
        if (CheckCollisionRecs(predictedPosY, environment->blocks[i]))
        {
            // falling
            if (player->velocity.y > 0) {
                newY = environment->blocks[i].y - player->shape.height;
                onGround = true;
            }
            else if (player->velocity.y < 0) // Jumping
            {
                newY = environment->blocks[i].y + environment->blocks[i].height;
            }
            player->velocity.y = 0;
            break;
        }
        player->useGravity = true;
    }

    // Update the player's position with the new values
    player->shape.y = newY;

    player->useGravity = !onGround;

    // Jumping (example: if space is pressed and the player is on the ground)
    if (IsKeyDown(KEY_SPACE) && onGround)
    {
        player->velocity.y = -JUMP_SPEED;  // Ensure JUMP_SPEED is defined appropriately
    }
}
