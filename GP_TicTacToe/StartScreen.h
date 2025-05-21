#ifndef StartScreen__H
#define StartScreen__H

#include <SDL.h>
#include <cmath>
#include "graphics.h"
#include "game.h"

namespace __StartScreen_internal
{
    extern SDL_Texture* startButton;

    extern int buttonX,buttonY;
    extern int buttonWidth,buttonHeight;

    extern int x,y;
    extern int alpha;

    extern float amplitude;
    extern float speed;

    extern bool quit;
    extern bool isClick;
    extern SDL_Event event;
}


void generateStartButton(Graphic &graphics);

#endif // StartScreen__H
