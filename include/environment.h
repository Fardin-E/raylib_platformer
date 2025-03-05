#pragma once

#include "raylib.h"
#include <vector>
#include <iostream>

class Environment {
    public:
    std::vector<Rectangle> blocks;

    Environment(const std::vector<Rectangle> &blockList);

    void InitAndDrawFloor(Color tint) const;
    void DrawBlocks(Vector2 origin, float rotation, Color tint) const;
};
