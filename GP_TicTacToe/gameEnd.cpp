//gameEnd.cpp
#include "gameEnd.h"

bool clicked;

SDL_Cursor* arrowCursor;
SDL_Cursor* handCursor;
SDL_Cursor* currentCursor;

using namespace std;

void generateScoreScreen(Graphic &graphics)
{
    //Nút chơi lại
    int buttonX=graphics.centerTextPos(playAgainButton);
    int buttonY=500;
    int buttonWidth,buttonHeight;
    SDL_QueryTexture(playAgainButton,nullptr,nullptr,&buttonWidth,&buttonHeight);

    int x,y;

    clicked=false;
    SDL_Event playAgain;
    while(!clicked)
    {
        while(SDL_PollEvent(&playAgain))
        {
            if(playAgain.type==SDL_QUIT)
            {
                exit(0);
            }
            if(playAgain.type==SDL_MOUSEBUTTONDOWN&&playAgain.button.button==SDL_BUTTON_LEFT)
            {
                //Kiểm tra nút có được ấn hay không
                SDL_GetMouseState(&x,&y);
                if(x>=buttonX&&x<=buttonX+buttonWidth&&y>=buttonY&&y<=buttonY+buttonHeight)
                {
                    clicked=true;
                }
            }
        }

        //Hiển thị màn hình và ống sau khi kết thúc game
        graphics.renderScrolling(ScrollFlappyBack);
        for(auto& pipe:pipes)
        {
            pipe.renderPipes(graphics.renderer);
        }

        //Hiển thị cỏ để che ống
        graphics.renderTexture(grass.texture,grass.scrollingOffSet,510);
        graphics.renderTexture(grass.texture,grass.scrollingOffSet-grass.width,510);

        //Viền đen outline của bảng điểm
        SDL_Rect rect1= {SCREEN_WIDTH/2-253,SCREEN_HEIGHT/2-153,506,306};
        SDL_SetRenderDrawColor(graphics.renderer,0,0,0,255);
        SDL_RenderFillRect(graphics.renderer,&rect1);

        //Viền vàng của bảng điểm
        SDL_Rect rect2= {SCREEN_WIDTH/2-250,SCREEN_HEIGHT/2-150,500,300};
        SDL_SetRenderDrawColor(graphics.renderer,255, 240, 112,255);
        SDL_RenderFillRect(graphics.renderer,&rect2);

        //In ra màn hình chữ "SCORES"
        texture=graphics.renderText("SCORES",text,black);
        graphics.renderTexture(texture,graphics.centerTextPos(texture),SCREEN_HEIGHT/2-120);

        //In ra điểm của người chơi
        string tg=to_string(scores);
        scoreTexture=graphics.renderText(tg.c_str(),scoreText,black);
        graphics.renderTexture(scoreTexture,graphics.centerTextPos(scoreTexture),SCREEN_HEIGHT/2-80);

        //In ra màn hình chữ "HIGHSCORE"
        texture=graphics.renderText("HIGHSCORE",text,black);
        graphics.renderTexture(texture,graphics.centerTextPos(texture),SCREEN_HEIGHT/2);

        //In ra điểm cao nhất của người chơi
        string diemCao=to_string(loadHighScore());
        scoreTexture=graphics.renderText(diemCao.c_str(),scoreText,black);
        graphics.renderTexture(scoreTexture,graphics.centerTextPos(scoreTexture),SCREEN_HEIGHT/2+40);

        graphics.hover(playAgainButton,buttonX,buttonY,currentCursor);

        graphics.presentScene();
        SDL_Delay(16);
    }

    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));

    if(scores>loadHighScore())
    {
        saveHighScore(scores);
    }
}
