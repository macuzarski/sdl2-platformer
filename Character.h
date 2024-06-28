//
// Created by macuz on 28.06.2024.
//
#include <SDL.h>
#include <vector>

#include "LTexture.h"
#ifndef CHARACTER_H
#define CHARACTER_H

//The character that will move around on the screen
class Character
{
public:
    //The dimensions of the character
    static const int CHARACTER_WIDTH = 20;
    static const int CHARACTER_HEIGHT = 20;

    //Maximum axis velocity of the character
    static const int CHARACTER_VEL = 5;

    //Initializes the variables
    Character(SDL_Renderer* render, LTexture* texture, int x, int y );

    //Takes key presses and adjusts the character's velocity
    void handleEvent( SDL_Event& e );

    //Moves the character
    void move( std::vector<SDL_Rect>& otherColliders );

    //Shows the character on the screen
    void render();

    //Gets the collision boxes
        std::vector<SDL_Rect>& getColliders();

private:
    //The X and Y offsets of the character
    int mPosX, mPosY;

    //The velocity of the character
    int mVelX, mVelY;

    SDL_Renderer* renderer;
    LTexture* gCharacterTexture;
    //character's collision box

    //character's collision boxes
    std::vector<SDL_Rect> mColliders;

    //Moves the collision boxes relative to the character's offset
    void shiftColliders();
};

#endif //CHARACTER_H
