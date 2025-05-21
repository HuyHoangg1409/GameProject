//game.cpp
#include "game.h"

scrollingBackground ScrollFlappyBack;
scrollingBackground grass;

SDL_Texture* outline_title_FlappyBird;
SDL_Texture* title_FlappyBird;
SDL_Texture* scoreTexture;
SDL_Texture* texture;
SDL_Texture* texPipe;
SDL_Texture* playAgainButton;
SDL_Texture* gamePause;
SDL_Texture* pausedButton;
SDL_Texture* resumeButton;
SDL_Texture* restartButton;
SDL_Texture* gameContinue;

Sprite bird;

TTF_Font* outlineTitle;
TTF_Font* gFont;
TTF_Font* scoreText;
TTF_Font* text;

Mix_Music* themeSong;
Mix_Chunk* scoreSound;
Mix_Chunk* jumpSound;
Mix_Chunk* dieSound;
Mix_Chunk* hitSound;

void initSources(Graphic &graphics)
{
    ScrollFlappyBack.setTexture(graphics.loadingTexture(BACKGROUND_SCROLLING));

    //OutlineText
    outlineTitle=graphics.loadFont(DEFAULT_FONT_FILE,TITLE_FONT_SIZE);
    TTF_SetFontOutline(outlineTitle,8);
    outline_title_FlappyBird=graphics.renderText("Flappy Bird",outlineTitle,black);

    //innerText
    gFont=graphics.loadFont(DEFAULT_FONT_FILE,TITLE_FONT_SIZE);
    title_FlappyBird=graphics.renderText("Flappy Bird",gFont,light_green);

    //scoresText
    scoreText=graphics.loadFont(DEFAULT_FONT_FILE,50);

    text=graphics.loadFont(DEFAULT_FONT_FILE,30);


    //initBird
    bird.texture=graphics.loadingTexture(BIRD_SPRITE_FILE);
    bird.init(bird.texture,BIRD_FRAMES,BIRD_CLIPS);

    //Layer Grass
    grass.setTexture(graphics.loadingTexture("images/Grass3.png"));

    playAgainButton=graphics.loadingTexture("images/playAgainButton1.png");
    gamePause=graphics.loadingTexture("images/pause.png");
    gameContinue=graphics.loadingTexture("images/continue.png");
    pausedButton=graphics.loadingTexture("images/gamePaused.png");
    resumeButton=graphics.loadingTexture("images/resume.png");
    restartButton=graphics.loadingTexture("images/restart.png");

    texPipe=graphics.loadingTexture(PIPE_FILE);

    //Sound when bird jump
    jumpSound=graphics.loadSound(JUMP_SOUND_FILE);

    //Sound when bird score
    scoreSound=graphics.loadSound(SCORE_SOUND);

    //Sound when bird die
    dieSound=graphics.loadSound(DIE_SOUND);
    hitSound=graphics.loadSound(HIT_SOUND);

    //Theme song
    themeSong=graphics.loadMusic(THEME_SONG);
}

void waitUntilKeyPressed()
{
    SDL_Event event;
    while(true)
    {
        if(SDL_PollEvent(&event)!=0&&(event.type==SDL_QUIT||event.type==SDL_KEYDOWN))
        {
            return;
        }
        SDL_Delay(10);
    }
}

Pipe::Pipe(SDL_Texture* _texture,int startX)
{
    texture=_texture;
    x=startX;
    y=rand()%200+100;
    checked=false;
}

void Pipe::update()
{
    if(x+width<0)
    {
        x=1400;
        y=rand()%200+70;
        checked=false;
    }
    x-=speed;
}

void Pipe::renderPipes(SDL_Renderer* renderer)
{
    SDL_Rect topPipe={x,y-height,width,height};
    SDL_Rect bottomPipe={x,y+gap,width,height};
    SDL_RenderCopyEx(renderer,texture,nullptr,&topPipe,0,nullptr,SDL_FLIP_VERTICAL);
    SDL_RenderCopy(renderer,texture,nullptr,&bottomPipe);
}

bool Pipe::checkCollision(int birdX,int birdY)
{
    int Nx,Ny1,Ny2;
    Nx=std::max(x,std::min(birdX,x+width));
    Ny1=std::max(y-height,std::min(birdY,y))-2;
    Ny2=std::max(y+gap,std::min(birdY,SCREEN_HEIGHT-150))-2;

    int dx=(Nx-birdX);
    int dy1=(Ny1-birdY);
    int dy2=(Ny2-birdY);
    int distance1=dx*dx+dy1*dy1;
    int distance2=dx*dx+dy2*dy2;

    if(distance1<BIRD_RADIUS*BIRD_RADIUS||distance2<BIRD_RADIUS*BIRD_RADIUS)
    {
        return true;
    }
    return false;
}

void Bird::updatePosition()
{
    velocity+=gravity;
    y+=velocity;

    if(y<0)
    {
        y=0;
    }

    if(y+BIRD_HEIGHT>SCREEN_HEIGHT)
    {
        y=SCREEN_HEIGHT-BIRD_HEIGHT;
        velocity=0;
    }
}

bool Pipe::checkScores(int birdX)
{
    if(birdX>=x+width-BIRD_WIDTH)
    {
        return true;
    }
    return false;
}
