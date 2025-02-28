#pragma once

#include "raylib.h"
#include <stdlib.h>

typedef struct Environment
{
    Rectangle *blocks;
    int blockNum;
} Environment;


Environment CreateEnvironment(Rectangle blocks[], int blockNum);

void Init_and_draw_floor(Environment *environment, Color tint);

void drawBlock(Environment *environment, Vector2 origin, float rotation, Color tint);
