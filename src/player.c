#include "player.h"

Player *CreatePlayer(Rectangle shape, Vector2 velocity, Camera2D camera,
    PlayerState state, SpriteAnimation animation_array[], int array_length)
{
    Player *player = malloc(sizeof(Player));
    if (!player)
    {
        TraceLog(LOG_WARNING, "Failed to allocate memory for Player");
        return NULL;
    }

    *player = (Player){
        .shape = shape,
        .velocity = velocity,
        .camera = camera,
        .state = state,
        .animation_array = NULL,
        .array_length = array_length,
    };

    player->animation_array = malloc(sizeof(SpriteAnimation) * array_length);
    if (!player->animation_array)
    {
        TraceLog(LOG_WARNING, "Failed to allocate memory for animation array");
        free(player);  // Free the player struct before returning
        return NULL;
    }

    for (int i = 0; i < array_length; i++)
    {
        player->animation_array[i] = animation_array[i]; // Deep copy animations
    }

    return player;
}

void DisposePlayer(Player *player)
{
    if (player == NULL) return;

    // Dispose of each SpriteAnimation inside the player's animation array.
    for (int i = 0; i < player->array_length; i++)
    {
        // DisposeSpriteAnimation should free the memory allocated for rectangles.
        DisposeSpriteAnimation(&player->animation_array[i]);
    }

    // Free the animation array.
    free(player->animation_array);

    // Free the player struct itself.
    free(player);
}

SpriteAnimation CreateSpriteAnimation(Texture2D atlas, int framesPerSecond, 
    Rectangle rectangles[], int length)
{
    SpriteAnimation spriteAnimation = 
    {
        .atlas = atlas,
        .framesPerSecond = framesPerSecond,
        .timeStarted = GetTime(),
        .rectangles = NULL,
        .rectanglesLength = length,
    };

    Rectangle *mem = malloc(sizeof(Rectangle) * length);
    if (mem == NULL)
    {
        TraceLog(LOG_WARNING, "No memory for CreateSpriteAnimation");
        spriteAnimation.rectanglesLength = 0;
        return spriteAnimation;
    }

    spriteAnimation.rectangles = mem;

    for (int i = 0; i < length; i++)
    {
        spriteAnimation.rectangles[i] = rectangles[i];
    }

    return spriteAnimation;
}

void DisposeSpriteAnimation(SpriteAnimation *animation)
{
    if (animation == NULL) return;

    if (animation->rectangles)
    {
        free(animation->rectangles);
        animation->rectangles = NULL;
    }
}

void DrawPlayer(Player *player, float rotation, float dt, Vector2 origin, Color tint)
{
    if (player == NULL || player->animation_array == NULL)
        return;

    for (int i = 0; i < player->array_length; i++)
    {
        const SpriteAnimation *anime = &player->animation_array[i];

        // Safety checks
        if (anime->rectangles == NULL || anime->rectanglesLength == 0)
            continue;

        int index = (int)((GetTime() - anime->timeStarted) * anime->framesPerSecond) % anime->rectanglesLength;
        Rectangle source = anime->rectangles[index];

        DrawTexturePro(anime->atlas, source, player->shape, origin, rotation, tint);
    }
}

void PlayerMovement(Player *player, float dt, float acceleration, float max_speed)
{
    // Calculate whether we're using ground or air movement
    bool isInAir = (player->state == JUMPING || player->state == FREE_FALLING);
    float frictionValue = isInAir ? AIR_DRAG : GROUND_FRICTION;

    if (IsKeyDown(KEY_RIGHT))
    {
        // Apply acceleration with higher values for responsiveness
        player->velocity.x += acceleration * dt;
        if (player->velocity.x > max_speed) player->velocity.x = max_speed;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        // Apply acceleration with higher values for responsiveness
        player->velocity.x -= acceleration * dt;
        if (player->velocity.x < -max_speed) player->velocity.x = -max_speed;
    }
    else
    {
        // Apply appropriate friction based on whether in air or on ground
        player->velocity.x *= frictionValue;

        // Stop completely if movement is very small
        if (fabsf(player->velocity.x) < 5.0f) { // Higher threshold for stopping
            player->velocity.x = 0.0f;
        }
    }
}


void UpdatePlayerCollisionAndState(Player *player, Environment *environment, float dt)
{
    // Store previous state for animation transitions
    PlayerState previousState = player->state;

    // Apply movement before calculating position
    if (player->state != GROUNDED) {
        player->velocity.y += GRAVITY * dt;
        PlayerMovement(player, dt, AIR_SPEED, MAX_SPEED);
    }
    else {
        PlayerMovement(player, dt, SPEED, MAX_SPEED);
    }

    // Calculate predicted positions AFTER updating velocities
    float newX = player->shape.x + player->velocity.x * dt;
    float newY = player->shape.y + player->velocity.y * dt;
    bool collisionFound = false;

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
            // Transition to falling at peak of jump
            if (player->velocity.y > 0) {
                player->state = FREE_FALLING;
            }
            break;

        case FREE_FALLING:
        case HORIZONTAL_COLLISION:
        case VERTICAL_COLLISION:
            break;
    }

    // Update animation based on state change
    if (previousState != player->state) {
        // Animation state transition
    }
}