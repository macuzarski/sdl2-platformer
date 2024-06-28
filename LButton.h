//
// Created by macuz on 28.06.2024.
//

#ifndef LBUTTON_H
#define LBUTTON_H
#include <SDL.h>
//The mouse button
enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

class LButton
{
public:
    //Initializes internal variables
    LButton();

    //Sets top left position
    void setPosition( int x, int y );

    //Handles mouse event
    void handleEvent( SDL_Event* e );

    //Shows button sprite
    void render(SDL_Renderer* renderer);

private:
    //Top left position
    SDL_Point mPosition;

    //Currently used global sprite
    LButtonSprite mCurrentSprite;
};
#endif //LBUTTON_H
