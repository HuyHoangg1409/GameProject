#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "graphics.h"
#include "game.h"
#include "StartScreen.h"
#include "gameRun.h"
#include "gameEnd.h"

using namespace std;
using namespace __GameRun_internal;

int main(int argc,char* argv[])
{
    Graphic graphics;
    graphics.init(SCREEN_WIDTH,SCREEN_HEIGHT);
    initSources(graphics);

    SDL_Cursor* arrowCursor = nullptr;
    SDL_Cursor* handCursor = nullptr;
    SDL_Cursor* currentCursor = nullptr;

    do
    {
        generateStartButton(graphics);

        generateGame(graphics);

        generateScoreScreen(graphics);

        graphics.presentScene();
        SDL_RenderClear(graphics.renderer);
        cerr<<1;
    }
    while(clicked);


    waitUntilKeyPressed();

    TTF_CloseFont(outlineTitle);
    TTF_CloseFont(gFont);
    TTF_CloseFont(scoreText);
    TTF_CloseFont(text);

    Mix_FreeMusic(themeSong);
    Mix_FreeChunk(jumpSound);
    Mix_FreeChunk(scoreSound);
    Mix_FreeChunk(dieSound);
    Mix_FreeChunk(hitSound);

    graphics.destroyTexture(title_FlappyBird);
    graphics.destroyTexture(outline_title_FlappyBird);
    graphics.destroyTexture(ScrollFlappyBack.texture);
    graphics.destroyTexture(grass.texture);
    graphics.destroyTexture(bird.texture);
    graphics.destroyTexture(texture);
    graphics.destroyTexture(texPipe);
    graphics.destroyTexture(scoreTexture);

    graphics.quit();

    return 0;
}
//500 400
//700 460
