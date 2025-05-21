//StartScreen.cpp
#include "StartScreen.h"

namespace __StartScreen_internal
{
    SDL_Texture* startButton;

    int buttonX,buttonY;
    int buttonWidth,buttonHeight;

    int x,y;
    int alpha=255;

    float amplitude=10.0f;
    float speed=0.004f;

    bool quit;
    bool isClick;
    SDL_Event event;
}

using namespace __StartScreen_internal;

void generateStartButton(Graphic &graphics)
{
    startButton=graphics.loadingTexture("images/StartButton.png");

    //Lấy chiều cao và kích thước của nút Start
    buttonX=graphics.centerTextPos(startButton);
    buttonY=400;
    SDL_QueryTexture(startButton,nullptr,nullptr,&buttonWidth,&buttonHeight);

    isClick=false;
    quit=false;
    while(!quit&&!isClick)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type==SDL_QUIT||event.key.keysym.scancode==SDL_SCANCODE_ESCAPE)
            {
                quit=true;
            }
            if(event.type==SDL_MOUSEBUTTONDOWN&&event.button.button==SDL_BUTTON_LEFT)
            {
                //Kiểm tra nút có được ấn hay không
                SDL_GetMouseState(&x,&y);
                if(x>=buttonX&&x<=buttonX+buttonWidth&&y>=buttonY&&y<=buttonY+buttonHeight)
                {
                    isClick=true;
                }
            }
        }

        graphics.renderTexture(ScrollFlappyBack.texture,0,0);//Background

        float offSetY=amplitude*sin(SDL_GetTicks()*speed);//Làm chữ dao động trong khoảng amplitude với tốc độ speed

        //Vị trí của title Flappy Bird
        int textX=graphics.centerTextPos(title_FlappyBird);
        int textY=100+offSetY;

        //In ra màn hình title và outline của title
        graphics.renderTexture(outline_title_FlappyBird,textX-BIRD_WIDTH/2,textY);
        graphics.renderTexture(title_FlappyBird,textX-BIRD_WIDTH/2+4,textY+5);

        //Làm con chim chuyển động
        bird.tick();

        //Lấy kích thước title để in chim
        int textWidth,textHeight;
        SDL_QueryTexture(outline_title_FlappyBird,nullptr,nullptr,&textWidth,&textHeight);

        //In con chim ra màn hình
        graphics.renderAnimation(textX+textWidth,textY+textHeight/2-BIRD_HEIGHT/2-10,bird,1.25f);

        graphics.hover(startButton,buttonX,buttonY,currentCursor);

        graphics.presentScene();
        SDL_Delay(16);
    }

    SDL_RenderClear(graphics.renderer);
    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));

    graphics.destroyTexture(startButton);

    if(!isClick)
    {
        exit(0);
    }
}
