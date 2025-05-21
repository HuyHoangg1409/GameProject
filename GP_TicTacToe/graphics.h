#ifndef GRAPHICS__H
#define GRAPHICS__H

#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_Mixer.h>
#include <SDL_ttf.h>
#include "defs.h"

extern SDL_Cursor* arrowCursor;
extern SDL_Cursor* handCursor;
extern SDL_Cursor* currentCursor;

struct scrollingBackground
{
    SDL_Texture* texture;
    int scrollingOffSet=0;
    int width,height;

    void setTexture(SDL_Texture* _texture)
    {
        texture=_texture;
        SDL_QueryTexture(texture,nullptr,nullptr,&width,&height);
    }

    void scroll(int distance)
    {
        scrollingOffSet-=distance;
        if(scrollingOffSet<0)
        {
            scrollingOffSet=width;
        }
    }
};

struct Sprite
{
    SDL_Texture* texture;
    std::vector <SDL_Rect> clips;
    int currentFrame=0;
    float x,y;
    float velocity=0.0f;

    void init(SDL_Texture* _texture,int frames,const int _clips[][4])
    {
        texture=_texture;
        SDL_Rect clip;
        for(int i=0; i<frames; i++)
        {
            clip.x=_clips[i][0];
            clip.y=_clips[i][1];
            clip.w=_clips[i][2];
            clip.h=_clips[i][3];
            clips.push_back(clip);
        }
    }

    void tick()
    {
        static int frameDelay=0;
        frameDelay++;

        if(frameDelay>=5)
        {
            frameDelay=0;
            currentFrame=(currentFrame+1)%clips.size();
        }
    }

    void updatePosition()
    {
        velocity+=gravity;
        y+=velocity;

        if(y<0)
        {
            y=0;
        }

        if(y+BIRD_HEIGHT>SCREEN_HEIGHT-140)
        {
            y=SCREEN_HEIGHT-BIRD_HEIGHT-140;
            velocity=0;
        }
    }

    void jump()
    {
        velocity=jump_velocity;
    }

    const SDL_Rect* getCurrentClip() const
    {
        return &(clips[currentFrame]);
    }
};

struct Graphic
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    void logErrorAndExit(const char* msg,const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_ERROR,"%s: %s",msg,SDL_GetError());
        SDL_Quit();
        IMG_Quit();
        Mix_Quit();
        TTF_Quit();
        exit(EXIT_FAILURE);
    }

    void init(const int width,const int height)
    {
        if(SDL_Init(SDL_INIT_EVERYTHING)<0)
        {
            logErrorAndExit("SDL_INIT",SDL_GetError());
        }

        window=SDL_CreateWindow(WINDOW_TITLE,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);

        if(window==nullptr)
        {
            logErrorAndExit("SDL_CreateWindow",SDL_GetError());
        }

        int flags=IMG_INIT_JPG|IMG_INIT_PNG;
        if((IMG_Init(flags)&flags)!=flags)
        {
            logErrorAndExit("IMG_INIT",IMG_GetError());
        }

        renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

        arrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
        handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
        currentCursor = arrowCursor;
        SDL_SetCursor(currentCursor);  // Đặt con trỏ mặc định ban đầu

//      Use this when use computer at school
//      renderer=SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

        if(renderer==nullptr)
        {
            logErrorAndExit("SDL_CreateRenderer",SDL_GetError());
        }

//      Smooth the image when scaling
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"linear");

        SDL_RenderSetLogicalSize(renderer,width,height);

        if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0)
        {
            logErrorAndExit("Mix_OpenAudio",Mix_GetError());
        }

        if(TTF_Init()==-1)
        {
            logErrorAndExit("SDL_Fonts",TTF_GetError());
        }

    }

    SDL_Texture* loadingTexture(const char* filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,"Loading image... %s",filename);

        SDL_Surface* tempSurface=IMG_Load(filename);
        if(tempSurface==nullptr)
        {
            logErrorAndExit("Failed to load image to Surface",IMG_GetError());
        }

        SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer,tempSurface);
        SDL_FreeSurface(tempSurface);

        if(texture==nullptr)
        {
            logErrorAndExit("Failed to load image",IMG_GetError());
        }
        return texture;
    }

    void prepareScene(SDL_Texture* background)
    {
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,background,nullptr,nullptr);//Dung de ve 1 texture len 1 renderer
    }

    void presentScene()
    {
        SDL_RenderPresent(renderer);
    }

    void renderTexture(SDL_Texture* texture,int x,int y)//Ve 1 texture len man hinh tai vi tri (x,y)
    {
        SDL_Rect dest;
        dest.x=x;
        dest.y=y;

        SDL_QueryTexture(texture,nullptr,nullptr,&dest.w,&dest.h);//Lay kich thuoc cua texture gan vao dest

        SDL_RenderCopy(renderer,texture,nullptr,&dest);
    }

    void blitRect(SDL_Texture* texture,int x,int y,int w,int h)
    {
        SDL_Rect dest;
        dest.x=x;
        dest.y=y;
        dest.w=w;
        dest.h=h;

        SDL_RenderCopy(renderer,texture,nullptr,&dest);
    }

    Mix_Music* loadMusic(const char* filename)
    {
        Mix_Music* gMusic=Mix_LoadMUS(filename);

        if(gMusic==nullptr)
        {
            logErrorAndExit("Cannot load music",Mix_GetError());
        }
        return gMusic;
    }

    void play(Mix_Music* gMusic)
    {
        if(gMusic==nullptr)
        {
            return;
        }
        if(Mix_PlayingMusic()==0)
        {
            Mix_PlayMusic(gMusic,-1);
        }
        else if(Mix_PlayingMusic()==1)
        {
            Mix_ResumeMusic();
        }
    }

    Mix_Chunk* loadSound(const char* filename)
    {
        Mix_Chunk* gChunk=Mix_LoadWAV(filename);

        if(gChunk==nullptr)
        {
            logErrorAndExit("Cannot load sound",Mix_GetError());
        }
        return gChunk;
    }

    void play(Mix_Chunk* gChunk)
    {
        if(gChunk==nullptr)
        {
            return;
        }
        Mix_PlayChannel(-1,gChunk,0);
    }

    TTF_Font* loadFont(const char* filename,int size)
    {
        TTF_Font* gFont=TTF_OpenFont(filename,size);

        if(gFont==nullptr)
        {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_ERROR,"Load font: %s",TTF_GetError());
        }
        return gFont;
    }

    SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color textColor)
    {
        SDL_Surface* textSurface=TTF_RenderText_Solid(font,text,textColor);
        if(textSurface==nullptr)
        {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_ERROR,"Render text surface %s",TTF_GetError());
            return nullptr;
        }

        SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer,textSurface);
        if(texture==nullptr)
        {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_ERROR,"Create texture from text %s",TTF_GetError());
        }

        SDL_FreeSurface(textSurface);
        return texture;
    }

    void destroyTexture(SDL_Texture* texture)
    {
        SDL_DestroyTexture(texture);
        texture=nullptr;
    }

    void quit()
    {
        TTF_Quit();
        Mix_Quit();
        IMG_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        SDL_FreeCursor(arrowCursor);
        SDL_FreeCursor(handCursor);
    }

    int centerTextPos(SDL_Texture* texture)
    {
        int titleWidth;
        SDL_QueryTexture(texture,nullptr,nullptr,&titleWidth,nullptr);
        return (SCREEN_WIDTH-titleWidth)/2;
    }

    void renderScrolling(const scrollingBackground& background)
    {
        renderTexture(background.texture,background.scrollingOffSet,0);
        renderTexture(background.texture,background.scrollingOffSet-background.width,0);
    }

    void renderAnimation(int x,int y,const Sprite& sprite,float scale=1.0f)
    {
        const SDL_Rect* clip=sprite.getCurrentClip();
        SDL_Rect renderQuad= {x,y,int(clip->w*scale),int(clip->h*scale)};
        SDL_RenderCopy(renderer,sprite.texture,clip,&renderQuad);
    }

    void hover(SDL_Texture* texture,int buttonX,int buttonY,SDL_Cursor*& currentCursor)
    {
        int x,y,alpha;
        int buttonWidth,buttonHeight;
        SDL_QueryTexture(texture,nullptr,nullptr,&buttonWidth,&buttonHeight);

        //Tạo hiệu ứng nút ấn được
        SDL_GetMouseState(&x,&y);
        if(x>=buttonX&&x<=buttonX+buttonWidth&&y>=buttonY&&y<=buttonY+buttonHeight)
        {
            if(currentCursor!=handCursor)
            {
                SDL_SetCursor(handCursor);
                currentCursor=handCursor;
            }
            alpha=220;
        }
        else
        {
            if(currentCursor!=arrowCursor)
            {
                SDL_SetCursor(arrowCursor);
                currentCursor=arrowCursor;
            }
            alpha=255;
        }

        //Làm mờ nút và in nút ra màn hình
        SDL_SetTextureAlphaMod(texture,alpha);
        renderTexture(texture,buttonX,buttonY);
    }

    bool isMouseInside(SDL_Texture* texture, int x, int y)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

        return (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h);
    }
};

#endif // GRAPHICS__H
