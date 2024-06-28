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

    //Set color modulation
    void setColor( Uint8 red, Uint8 green, Uint8 blue );

    //Set blending
    void setBlendMode( SDL_BlendMode blending );

    //Set alpha modulation
    void setAlpha( Uint8 alpha );

    //Renders texture at given point
    void renderB(SDL_Renderer* render, int x, int y);
    void render( SDL_Renderer* render,int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
    //Gets image dimensions
    int getWidth();
    int getHeight();
    std::string getTexturePath();

private:
    //The actual hardware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
    std::string texturePath;


};

#endif //LTEXTURE_H
