//
// Created by macuz on 27.06.2024.
//
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#ifndef LTEXTURE_H
#define LTEXTURE_H

//Texture wrapper class
class LTexture
{
public:
    //Initializes variables
    explicit LTexture();

    //Deallocates memory
    ~LTexture();

    //Loads image at specified path
    bool loadFromFile( std::string path, SDL_Renderer* renderer );
    //Creates image from font string
    bool loadFromRenderedText( std::string textureText, SDL_Color textColor, SDL_Renderer* render, TTF_Font* gFont );

    //Deallocates texture
    void free();

    //Renders texture at given point
    void render(SDL_Renderer* render, int x, int y);
    void renderEx( SDL_Renderer* render,int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
    //Gets image dimensions
    int getWidth();
    int getHeight();

private:
    //The actual hardware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};

#endif //LTEXTURE_H
