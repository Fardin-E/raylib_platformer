#include "environment.h"

Environment CreateEnvironment(Rectangle blocks[], int blockNum)
{
    Environment environment =
    {
          .blocks = NULL,
          .blockNum = blockNum
    };

    Rectangle *mem = malloc(sizeof(Rectangle) * blockNum);
    if (mem == NULL)
    {
          TraceLog(LOG_FATAL, "No memory for Create environment");
          environment.blockNum = 0;
          return environment;
    }

    environment.blocks = mem;

    for (int i = 0; i < blockNum; i++)
    {
          environment.blocks[i] = blocks[i];
    }

    return environment;
}

void Init_and_draw_floor(Environment *environment, Rectangle rectangle)
{
      if (environment == NULL || environment->blocks == NULL)
      {
            TraceLog(LOG_WARNING, "Environment or blocks array is NULL");
            return;
      }

      if (environment->blockNum > 0)
      {
            environment->blocks[0] = rectangle;

            DrawRectangleRec(environment->blocks[0], BLACK);

            // Optional: Draw outline to make sure it's visible
            DrawRectangleLinesEx(environment->blocks[0], 2, RED);
      }
      else
      {
            TraceLog(LOG_WARNING, "No blocks to draw");
      }
}

