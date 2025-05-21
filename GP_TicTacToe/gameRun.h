#ifndef GAMERUN__H
#define GAMERUN__H

#include <SDL.h>
#include "graphics.h"
#include "game.h"

namespace __GameRun_internal
{
extern int birdStateX,birdStateY;
extern int centerX,centerY;

extern float angle;

extern bool paused;
extern bool startGame;
extern bool jumpWhenLose;
extern bool gameEnd;

extern SDL_Event click;
}

extern int scores;
extern std::vector<Pipe> pipes;

void generateGame(Graphic &graphics);

#endif // GAMERUN__H
