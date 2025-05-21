#ifndef GAME__H
#define GAME__H

#include <algorithm>
#include <SDL.h>
#include "graphics.h"

extern scrollingBackground ScrollFlappyBack;
extern scrollingBackground grass;
extern SDL_Texture* outline_title_FlappyBird;
extern SDL_Texture* title_FlappyBird;
extern SDL_Texture* scoreTexture;
extern SDL_Texture* texture;
extern SDL_Texture* texPipe;
extern SDL_Texture* playAgainButton;
extern SDL_Texture* gamePause;
extern SDL_Texture* pausedButton;
extern SDL_Texture* resumeButton;
extern SDL_Texture* restartButton;
extern SDL_Texture* gameContinue;

extern Sprite bird;

extern TTF_Font* outlineTitle;
extern TTF_Font* gFont;
extern TTF_Font* scoreText;
extern TTF_Font* text;

extern Mix_Music* themeSong;
extern Mix_Chunk* scoreSound;
extern Mix_Chunk* jumpSound;
extern Mix_Chunk* dieSound;
extern Mix_Chunk* hitSound;

void initSources(Graphic &graphics);
void waitUntilKeyPressed();

struct Pipe
{
    SDL_Texture* texture;
    int x,y;
    const int width=130;
    const int height=300;
    const int gap= 200;
    const int speed=3;
    bool checked;

    Pipe(SDL_Texture* _texture,int startX);
    void update();
    void renderPipes(SDL_Renderer* renderer);
    bool checkCollision(int birdX,int birdY);
    bool checkScores(int birdX);
};

struct Bird
{
    Sprite innerBird;
    float x,y;
    float velocity;

    void updatePosition();
};

#endif // GAME__H
