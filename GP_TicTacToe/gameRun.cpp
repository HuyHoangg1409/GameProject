//gameRun.cpp
#include <iostream>
#include <algorithm>
#include "gameRun.h"

namespace __GameRun_internal
{
int birdStateY;
int birdStateX;
int centerX;
int centerY;

float angle;

bool paused;
bool startGame;
bool jumpWhenLose;
bool gameEnd;

SDL_Event click;
}

int scores;//Khoi tao diem ban dau
std::vector<Pipe> pipes;

using namespace __GameRun_internal;
using namespace std;

void generateGame(Graphic &graphics)
{
    //khởi tạo giá trị
    scores=0;
    birdStateY=SCREEN_HEIGHT/2-BIRD_HEIGHT;
    birdStateX=SCREEN_WIDTH/4;
    centerX=birdStateX+35;
    angle=0.0f;
    paused=false;
    startGame=false;
    jumpWhenLose=false;
    gameEnd=false;
    pipes.clear();

    //Tạo ống
    pipes.push_back(Pipe(texPipe,800+600));
    pipes.push_back(Pipe(texPipe,1100+600));
    pipes.push_back(Pipe(texPipe,1350+600));
    pipes.push_back(Pipe(texPipe,1700+600));
    pipes.push_back(Pipe(texPipe,2000+600));

    bird.x=birdStateX;
    bird.y=birdStateY;

    int x,y,alpha=255;
    bool hv1=false;
    bool hv2=false;
    bool hv3=false;

    int w1,h1;
    int w2,h2;
    SDL_QueryTexture(restartButton,nullptr,nullptr,&w1,&h1);
    SDL_QueryTexture(resumeButton,nullptr,nullptr,&w2,&h2);

    while(!gameEnd||birdStateY<=SCREEN_HEIGHT)
    {
        while(SDL_PollEvent(&click))
        {
            if(click.type == SDL_QUIT)
            {
                gameEnd = true;
            }

            if(click.type == SDL_MOUSEBUTTONDOWN && click.button.button == SDL_BUTTON_LEFT)
            {
                SDL_GetMouseState(&x, &y);

                if (paused)
                {
                    if (graphics.isMouseInside(restartButton,SCREEN_WIDTH/2-w1-50,SCREEN_HEIGHT/2-h1/2))
                    {
                        generateGame(graphics);
                        return;
                    }
                    else if (graphics.isMouseInside(resumeButton,SCREEN_WIDTH/2+50,SCREEN_HEIGHT/2-h2/2))
                    {
                        paused = false;
                    }
                    else if (graphics.isMouseInside(gamePause, 20, 20))
                    {
                        paused = false;
                    }
                }
                else
                {
                    if (x >= 20 && x <= 70 && y >= 20 && y <= 70)
                    {
                        paused = true;
                    }
                    else
                    {
                        graphics.play(jumpSound);
                        angle = -45.0f;
                        bird.jump();
                        startGame = true;
                    }
                }
            }

            if(click.type == SDL_KEYDOWN && click.key.keysym.scancode == SDL_SCANCODE_ESCAPE && !click.key.repeat)
            {
                paused = !paused;
            }
        }


        if(!gameEnd&&!paused)
        {
            ScrollFlappyBack.scroll(5);
            bird.tick();
        }
        graphics.renderScrolling(ScrollFlappyBack);

        if(startGame&&!gameEnd&&!paused)
        {
            //graphics.play(themeSong);
            bird.updatePosition();

            birdStateY=bird.y;
            centerY=bird.y+BIRD_HEIGHT/2;

            //Chim rơi dần
            angle+=2.0f;
            angle=std::min(angle,90.0f);
            if(bird.y+BIRD_HEIGHT==SCREEN_HEIGHT-140)
            {
                angle=0.0f;
            }

            for(auto &pipe:pipes)
            {
                pipe.update();
                pipe.renderPipes(graphics.renderer);
                if(pipe.checkCollision(centerX,centerY))
                {
                    gameEnd=true;
                }
                if(pipe.checkScores(bird.x)&&pipe.checked==false)
                {
                    graphics.play(scoreSound);
                    scores++;
                    pipe.checked=true;
                    std::cerr<<scores<<xd;
                }
            }


        }
        if(gameEnd)
        {
            if(!jumpWhenLose)
            {
                //Tắt nhạc nền sau khi thua
                if(Mix_PlayingMusic())
                {
                    Mix_HaltMusic();
                }

                //Tạo hiệu ứng chim rơi khi đâm vào ống
                birdStateY-=30;
                angle+=45;

                SDL_SetRenderDrawColor(graphics.renderer,255,255,255,255);
                SDL_RenderClear(graphics.renderer);
                SDL_RenderPresent(graphics.renderer);
                graphics.play(hitSound);
                graphics.play(dieSound);
            }
            jumpWhenLose=true;
            if(jumpWhenLose)
            {
                angle+=90;
                angle=min(angle,90.0f);
                SDL_Delay(5);
                birdStateY+=7;
            }
        }

        if(!gameEnd&&!paused)
        {
            grass.scroll(3);
        }
        for(auto& pipe:pipes)
        {
            pipe.renderPipes(graphics.renderer);
        }

        const SDL_Rect* clip=bird.getCurrentClip();
        SDL_Rect renderQuad= {birdStateX,birdStateY,int(clip->w*1.0f),int(clip->h*1.0f)};
        SDL_RenderCopyEx(graphics.renderer,bird.texture,clip,&renderQuad,angle,nullptr,SDL_FLIP_NONE);

        string tg=to_string(scores);
        scoreTexture=graphics.renderText(tg.c_str(),scoreText,black);
        graphics.renderTexture(scoreTexture,SCREEN_WIDTH/2-10,30);

        graphics.renderTexture(grass.texture,grass.scrollingOffSet,510);
        graphics.renderTexture(grass.texture,grass.scrollingOffSet-grass.width,510);

        if(!paused)
        {
            graphics.hover(gamePause,20,20,currentCursor);
        }
        else
        {
            //Làm mờ màn hình khi tạm dừng
            SDL_SetRenderDrawBlendMode(graphics.renderer,SDL_BLENDMODE_BLEND);//Dùng để alpha có tác dụng
            SDL_SetRenderDrawColor(graphics.renderer,0,0,0,100);
            SDL_Rect screen= {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
            SDL_RenderFillRect(graphics.renderer,&screen);

            //Viền đen outline của bảng
            SDL_Rect rect1= {SCREEN_WIDTH/2-253,SCREEN_HEIGHT/2-153,506,306};
            SDL_SetRenderDrawColor(graphics.renderer,0,0,0,255);
            SDL_RenderFillRect(graphics.renderer,&rect1);

            //Viền vàng của bảng
            SDL_Rect rect2= {SCREEN_WIDTH/2-250,SCREEN_HEIGHT/2-150,500,300};
            SDL_SetRenderDrawColor(graphics.renderer,255, 240, 112,255);
            SDL_RenderFillRect(graphics.renderer,&rect2);

            graphics.renderTexture(pausedButton,graphics.centerTextPos(pausedButton),SCREEN_HEIGHT/2-200);
            SDL_GetMouseState(&x,&y);
            hv1=graphics.isMouseInside(restartButton,SCREEN_WIDTH/2-w1-50,SCREEN_HEIGHT/2-h1/2);
            hv2=graphics.isMouseInside(resumeButton,SCREEN_WIDTH/2+50,SCREEN_HEIGHT/2-h2/2);
            hv3=graphics.isMouseInside(gamePause,20,20);

            if(hv1||hv2||hv3)
            {
                if(currentCursor!=handCursor)
                {
                    SDL_SetCursor(handCursor);
                    currentCursor=handCursor;
                }
            }
            else
            {
                if(currentCursor!=arrowCursor)
                {
                    SDL_SetCursor(arrowCursor);
                    currentCursor=arrowCursor;
                }
            }
            //Làm mờ nút và in nút ra màn hình
            SDL_SetTextureAlphaMod(resumeButton,alpha);
            SDL_SetTextureAlphaMod(restartButton,alpha);
            SDL_SetTextureAlphaMod(gameContinue,alpha);
            graphics.renderTexture(restartButton,SCREEN_WIDTH/2-w1-50,SCREEN_HEIGHT/2-h1/2);
            graphics.renderTexture(resumeButton,SCREEN_WIDTH/2+50,SCREEN_HEIGHT/2-h2/2);
            graphics.renderTexture(gameContinue,20,20);
        }

        graphics.presentScene();
        SDL_Delay(16);
    }
}
