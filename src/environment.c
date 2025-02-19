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

void Init_and_draw_floor(Environment *environment, Color tint)
{
      if (environment == NULL || environment->blocks == NULL)
      {
            TraceLog(LOG_WARNING, "Environment or blocks array is NULL");
            return;
      }

      if (environment->blockNum >= 0)
      {

            DrawRectangleRec(environment->blocks[0], tint);
      }
      else
      {
            TraceLog(LOG_WARNING, "No blocks to draw");
      }
}

void drawBlock(Environment *environment, Vector2 origin, float rotation, Color tint) 
{
    if (environment == NULL || environment->blocks == NULL)
    {
        TraceLog(LOG_WARNING, "Environment or blocks array is NULL");
        return;
    }

    if (environment->blockNum > 0)
    {
        // it is looping from i = 1 because i = 0 is hardcoded to be the floor
        for (int i = 1; i < environment->blockNum; i++)
        {
            DrawRectanglePro(environment->blocks[i], origin, rotation, tint);
        }
    }
    else
    {
        TraceLog(LOG_WARNING, "No blocks to draw");
    }
}

