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



void DrawPlayer(Player *player, float rotation, float dt, Vector2 origin, Color tint)
{
    if (player == NULL || player->animation_array == NULL)
        return;

    for (int i = 0; i < player->array_length; i++)
    {
        const SpriteAnimation *anim = &player->animation_array[i];

        // Safety checks
        if (anim->rectangles == NULL || anim->rectanglesLength == 0)
            continue;

        int index = (int)((GetTime() - anim->timeStarted) * anim->framesPerSecond) % anim->rectanglesLength;
        Rectangle source = anim->rectangles[index];

        DrawTexturePro(anim->atlas, source, player->shape, origin, rotation, tint);
    }
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



void UpdatePlayerCollisionAndState(Player *player, Environment *environment, float dt)
{
    // Movement
    if (IsKeyDown(KEY_RIGHT))
    {
        player->velocity.x = fminf(player->velocity.x * dt + SPEED, MAX_SPEED);
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        player->velocity.x = fmaxf(player->velocity.x * dt - SPEED, -MAX_SPEED);
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