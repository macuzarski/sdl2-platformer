//
// Created by macuz on 28.06.2024.
//
#include <SDL.h>
#ifndef CHARACTER_H
#define CHARACTER_H

//The dot that will move around on the screen
class Character
{
public:
    //The dimensions of the dot
    static const int CHARACTER_WIDTH = 20;
    static const int CHARACTER_HEIGHT = 20;

    //Maximum axis velocity of the dot
    static const int CHARACTER_VEL = 10;

    //Initializes the variables
    Character();

    //Takes key presses and adjusts the dot's velocity
    void handleEvent( SDL_Event& e );

    //Moves the dot
    void move();

    //Shows the dot on the screen
    void render();

private:
    //The X and Y offsets of the dot
    int mPosX, mPosY;

    //The velocity of the dot
    int mVelX, mVelY;
};

#endif //CHARACTER_H
