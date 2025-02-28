#include "player.h"

Player *CreatePlayer(Rectangle shape, Vector2 velocity, Camera2D camera,
    PlayerState state, PlayerDirection direction, SpriteAnimation animation_array[], int array_length)
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
        .direction = direction,
        .animation_array = NULL,
        .currentAnimation = ANIM_IDLE,
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
    Rectangle rectangles[], int length, AnimationType animeType)
{
    SpriteAnimation spriteAnimation = 
    {
        .atlas = atlas,
        .framesPerSecond = framesPerSecond,
        .timeStarted = GetTime(),
        .rectangles = NULL,
        .rectanglesLength = length,
        .type = animeType,
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

void DrawPlayer(Player *player, float rotation, float dt, Vector2 origin, Color tint) {
    if (player == NULL || player->animation_array == NULL)
        return;

    const SpriteAnimation *anime = &player->animation_array[player->currentAnimation];

    if (anime->rectangles == NULL || anime->rectanglesLength == 0)
        return;

    int index = (int)((GetTime() - anime->timeStarted) * anime->framesPerSecond) % anime->rectanglesLength;
    Rectangle source = anime->rectangles[index];

    DrawTexturePro(anime->atlas, source, player->shape, origin, rotation, tint);
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
        TraceLog(LOG_WARNING, "player velocity: %f and %f", player->velocity.x, player->shape.x);
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        // Apply acceleration with higher values for responsiveness
        player->velocity.x -= acceleration * dt;
        if (player->velocity.x < -max_speed) player->velocity.x = -max_speed;
        TraceLog(LOG_WARNING, "player velocity: %f and %f", player->velocity.x, player->shape.x);
    }
    else
    {
        // Apply appropriate friction based on whether in air or on ground
        player->velocity.x *= frictionValue;

        // Stop completely if movement is very small
        if (fabsf(player->velocity.x) < 0.1f) { 
            player->velocity.x = 0.0f;
        }
    }
}


void UpdatePlayerCollisionAndState(Player *player, Environment *environment, float dt)
{
    // Apply gravity and movement
    if (player->state != GROUNDED) {
        player->velocity.y += GRAVITY * dt;
        PlayerMovement(player, dt, AIR_SPEED, MAX_SPEED);
    }
    else {
        PlayerMovement(player, dt, SPEED, MAX_SPEED);
    }

    // Calculate predicted positions
    float newX = player->shape.x + player->velocity.x * dt;
    float newY = player->shape.y + player->velocity.y * dt;
    bool isOnGround = false;

    // Add a small buffer for ground detection (1-2 pixels)
    float groundCheckBuffer = 2.0f;
    
    // Check for ground directly below the player with a small buffer
    Rectangle groundCheckRect = {
        player->shape.x,
        player->shape.y + player->shape.height,
        player->shape.width,
        groundCheckBuffer
    };

    // First check if player is on ground
    for (int i = 0; i < environment->blockNum; i++) {
        if (CheckCollisionRecs(groundCheckRect, environment->blocks[i])) {
            isOnGround = true;
            break;
        }
    }

    // Then handle collisions
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
        }

        // Check vertical collision
        if (CheckCollisionRecs(predictedPosY, block))
        {
            if (player->velocity.y > 0) {  // Falling down and hit something below
                newY = block.y - player->shape.height;
                isOnGround = true;
            }
            else if (player->velocity.y < 0) {  // Moving up and hit something above
                newY = block.y + block.height;
            }
            player->velocity.y = 0;
        }
    }

    // Update positions
    player->shape.x = newX;
    player->shape.y = newY;


    // Add a small delay counter to prevent rapid state changes
    static int stateChangeDelay = 0;
    const int STATE_CHANGE_THRESHOLD = 5; // Adjust as needed

    // Keep consistent ground state with debouncing
    if (isOnGround) {
        // If we were in air but now on ground, add debounce delay
        if (player->state == JUMPING || player->state == FREE_FALLING) {
            stateChangeDelay = STATE_CHANGE_THRESHOLD;
        }
        
        // Only change state if jump is pressed or we're moving
        if (IsKeyDown(KEY_SPACE)) {
            player->velocity.y = -JUMP_SPEED;
            player->state = JUMPING;
            stateChangeDelay = 0; // Reset delay on explicit actions
        }
        else if (IsKeyDown(KEY_RIGHT) && fabsf(player->velocity.x) > 0.1f) {
            player->state = MOVING;
            player->direction = RIGHT;
            stateChangeDelay = 0; // Reset delay on explicit actions
        }
        else if (IsKeyDown(KEY_LEFT) && fabsf(player->velocity.x) > 0.1f) {
            player->state = MOVING;
            player->direction = LEFT;
            stateChangeDelay = 0; // Reset delay on explicit actions
        }
        else {
            player->state = GROUNDED;
        }
    }
    else if (stateChangeDelay > 0) {
        // Still consider on ground during debounce period
        stateChangeDelay--;
        // Don't change state during debounce
    }
    else {
        // Player is definitely in the air
        if (player->velocity.y < 0) {
            player->state = JUMPING;
        }
        else
        {
            player->state = FREE_FALLING;
        }
    }

    if (player->state == GROUNDED) {
        player->currentAnimation = ANIM_IDLE;
    }
    else if (player->state == MOVING && player->direction == RIGHT) {
        player->currentAnimation = ANIM_RUNR;
    }
    else if (player->state == MOVING && player->direction == LEFT) {
        player->currentAnimation = ANIM_RUNL;
    }
    else if (player->state == JUMPING && player->direction == RIGHT) {
        player->currentAnimation = ANIM_JUMPR;
    }
    else if (player->state == JUMPING && player->direction == LEFT) {
        player->currentAnimation = ANIM_JUMPL;
    }
    else if (player->state == FREE_FALLING && player->direction == LEFT) {
        player->currentAnimation = ANIM_JUMPL;
    }
    else if (player->state == FREE_FALLING && player->direction == RIGHT) {
        player->currentAnimation = ANIM_JUMPR;
    }
    else {
        player->currentAnimation = ANIM_IDLE; // Fallback
    }

}