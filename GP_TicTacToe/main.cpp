#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "def.h"
#include "graphics.h"

using namespace std;

void drawFilledCircle(SDL_Renderer* renderer, int x0, int y0, int radius);
void drawOutLinedCircle(SDL_Renderer* renderer, int x0, int y0, int radius);

void waitUntilKeyPressed();

void drawTable(Graphic &graphics)
{
    SDL_SetRenderDrawColor(graphics.renderer,0,0,0,255);
    SDL_RenderDrawLine(graphics.renderer,SCREEN_WIDTH/3,0,SCREEN_WIDTH/3,SCREEN_HEIGHT);
    SDL_RenderDrawLine(graphics.renderer,2*SCREEN_WIDTH/3,0,2*SCREEN_WIDTH/3,SCREEN_HEIGHT);
    SDL_RenderDrawLine(graphics.renderer,0,SCREEN_HEIGHT/3,SCREEN_WIDTH,SCREEN_HEIGHT/3);
    SDL_RenderDrawLine(graphics.renderer,0,2*SCREEN_HEIGHT/3,SCREEN_WIDTH,2*SCREEN_HEIGHT/3);
}

int gameRule(char a[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        if (a[i][0] == a[i][1] && a[i][1] == a[i][2] && a[i][0] != '0') return (a[i][0] == 'X') ? 1 : -1;
        if (a[0][i] == a[1][i] && a[1][i] == a[2][i] && a[0][i] != '0') return (a[0][i] == 'X') ? 1 : -1;
    }
    if (a[0][0] == a[1][1] && a[1][1] == a[2][2] && a[0][0] != '0') return (a[0][0] == 'X') ? 1 : -1;
    if (a[0][2] == a[1][1] && a[1][1] == a[2][0] && a[0][2] != '0') return (a[0][2] == 'X') ? 1 : -1;
    return 0;
}

int main(int argc, char* argv[])
{
    Graphic graphics;
    graphics.init();

    SDL_SetRenderDrawColor(graphics.renderer,255,255,255,255);
    SDL_RenderClear(graphics.renderer);

    drawTable(graphics);
    SDL_Texture* O_TicTacToe=graphics.loadingTexture("images/O-TicTacToe.png");
    SDL_Texture* X_TicTacToe=graphics.loadingTexture("images/X-TicTacToe.png");
    SDL_Texture* X_Win=graphics.loadingTexture("images/X_WIN.png");
    SDL_Texture* O_Win=graphics.loadingTexture("images/O_WIN.png");
    SDL_Texture* Draw=graphics.loadingTexture("images/DRAW.png");

    char a[3][3] = {{'0', '0', '0'},{'0', '0', '0'},{'0', '0', '0'}};
    SDL_Event event;
    int luot=0;
    bool check=false;
    while(SDL_WaitEvent(&event))
    {
        int x,y;
        int vtx,vty;
        if(gameRule(a)==1)
        {
            cerr<<"X Win"<<xd;
            check=true;
            SDL_Delay(700);
            graphics.renderTexture(X_Win,0,SCREEN_HEIGHT/2-200);
            break;
        }
        else if(gameRule(a)==-1)
        {
            cerr<<"O Win"<<xd;
            check=true;
            SDL_Delay(700);
            graphics.renderTexture(O_Win,0,SCREEN_HEIGHT/2-200);
            break;
        }
        if(luot==9)
        {
            break;
        }
        switch(event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x,&y);
            //cerr<<x<<" "<<y<<xd;
            vtx=int(x/(SCREEN_WIDTH/3));
            vty=int(y/(SCREEN_HEIGHT/3));
            cerr<<xd<<vtx<<" "<<vty<<xd;
            if(a[vty][vtx]=='0')
            {
                luot++;
                if(luot%2==1)
                {
                    a[vty][vtx]='X';
                    graphics.blitRect(X_TicTacToe,SCREEN_WIDTH/3*vtx,SCREEN_HEIGHT/3*vty,SCREEN_WIDTH/3,SCREEN_HEIGHT/3);
                }
                else
                {
                    a[vty][vtx]='O';
                    graphics.blitRect(O_TicTacToe,SCREEN_WIDTH/3*vtx,SCREEN_HEIGHT/3*vty,SCREEN_WIDTH/3,SCREEN_HEIGHT/3);
                }
            }
            for(int i=0; i<3; i++)
            {
                for(int j=0; j<3; j++)
                {
                    cerr<<a[i][j]<<" ";
                }
                cout<<xd;
            }
            break;
        case SDL_QUIT:
            exit(0);
            break;
        }
        graphics.presentScene();
        //SDL_Delay(100);
    }
    if(!check)
    {
        cerr<<"DRAW"<<xd;
        SDL_Delay(700);
        graphics.renderTexture(Draw,0,SCREEN_HEIGHT/2-200);
    }
    graphics.presentScene();
    waitUntilKeyPressed();

    X_TicTacToe=nullptr;
    SDL_DestroyTexture(X_TicTacToe);
    O_TicTacToe=nullptr;
    SDL_DestroyTexture(O_TicTacToe);

    graphics.quit();
    return 0;
}

void drawFilledCircle(SDL_Renderer* renderer, int x0, int y0, int radius)
{
    for (int i = x0 - radius; i <= x0 + radius; i++)
    {
        for (int j = y0 - radius; j <= y0 + radius; j++)
        {
            if ((i - x0) * (i - x0) + (j - y0) * (j - y0) <= radius * radius)
            {
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
    }
}

void drawOutLinedCircle(SDL_Renderer* renderer, int x0, int y0, int radius)
{
    int x = 0, y = radius;
    int p = 1 - radius;

    while (x <= y)
    {
        SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);

        //check the next point to draw
        x++;
        if (p < 0)
        {
            p += 2 * x + 3;
        }
        else
        {
            y--;
            p += 2 * (x - y) + 5;
        }
    }
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true)
    {
        if (SDL_PollEvent(&e) != 0 && (e.type == SDL_QUIT || e.type == SDL_KEYDOWN))
        {
            return;
        }
        SDL_Delay(100);
    }
}

