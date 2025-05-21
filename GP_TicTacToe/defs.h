#ifndef _DEFS__H
#define _DEFS__H

#include <SDL.h>

#define WINDOW_TITLE "MyProject"
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 675

#define TITLE_FONT_SIZE 100

extern const SDL_Color light_green;
extern const SDL_Color white;
extern const SDL_Color black;
extern const SDL_Color light_blue;

#define DEFAULT_FONT_FILE "assets/04B_19__.ttf"
#define JUMP_SOUND_FILE "assets/JumpEffect.wav"
#define THEME_SONG "assets/Theme_Song.mp3"
#define SCORE_SOUND "assets/audio_point.wav"
#define DIE_SOUND "assets/audio_die.wav"
#define HIT_SOUND "assets/audio_hit.wav"

#define BACKGROUND_SCROLLING "images/FlappyBird_Background.png"

#define BIRD_SPRITE_FILE "images/FlappyBird_fixed.png"
#define PIPE_FILE "images/topPipe.png"

extern const int BIRD_WIDTH;
extern const int BIRD_HEIGHT;
extern const int BIRD_CLIPS[][4];
extern const int BIRD_FRAMES;
extern const int BIRD_RADIUS;

extern const float jump_velocity;
extern const float gravity;

#define xd '\n'

#endif // _DEFS__H
