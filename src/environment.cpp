#include "environment.h"

Environment::Environment(const std::vector<Rectangle> &blockList)
    : blocks(blockList)
{
    if (blocks.empty()) {
        TraceLog(LOG_WARNING, "Environment initialized with no blocks.");
    }
}

void Environment::InitAndDrawFloor(Color tint) const {
    if (blocks.empty()) {
        TraceLog(LOG_WARNING, "No blocks available to draw the floor.");
        return;
    }

    // Draw the first block as the floor
    DrawRectangleRec(blocks[0], tint);
}

void Environment::DrawBlocks(Vector2 origin, float rotation, Color tint) const {
    if (blocks.empty()) {
        TraceLog(LOG_WARNING, "No blocks available to draw.");
        return;
    }

    // Start from index 1 because index 0 is the floor
    for (size_t i = 1; i < blocks.size(); i++) {
        DrawRectanglePro(blocks[i], origin, rotation, tint);
    }
}
