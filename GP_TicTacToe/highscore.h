#ifndef HIGHSCORE__H
#define HIGHSCORE__H

#include <fstream>

inline void saveHighScore(int highSCore)
{
    std::ofstream file("highscore.txt");
    if(file.is_open())
    {
        file<<highSCore;
        file.close();
    }
    else SDL_Log("Could not open highscore.txt");
}

inline int loadHighScore()
{
    std::ifstream file("highscore.txt");
    int highScore=0;
    if(file.is_open())
    {
        file>>highScore;
        file.close();
    }
    else SDL_Log("Could not open highscore.txt");
    return highScore;
}

#endif // HIGHSCORE__H
