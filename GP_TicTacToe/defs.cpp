//defs.cpp
#include "defs.h"

const SDL_Color light_green={158,234,88,255};
const SDL_Color white={255,255,255,255};
const SDL_Color black={68, 71, 64,255};
const SDL_Color light_blue={122, 249, 241,255};

const int BIRD_WIDTH=69;
const int BIRD_HEIGHT=48;
const int BIRD_CLIPS[][4]=
{
    {0, 0, 69, 48},
    {69, 0, 69, 48},
    {138, 0, 69, 48}
};
const int BIRD_FRAMES=sizeof(BIRD_CLIPS)/sizeof(int)/4;
const int BIRD_RADIUS=22;

const float jump_velocity=-8.0f;
const float gravity=0.4f;
