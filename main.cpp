//
// Created by macuz on 14.05.2024.
//

#include "defs.h"
#include "LTexture.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cmath>
#include <sstream>
#include <string>
#include "Character.h"
#include "LTimer.h"
#include "LButton.h"


//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;


enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};


SDL_Surface* loadSurface( std::string path );
//Starts up SDL and creates window
bool init();
//Loads media
bool loadMedia();
//Frees media and shuts down SDL
void close();
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;
//The image we will load and show on the screen
SDL_Surface* goatImg = NULL;
//The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
//Current displayed image
SDL_Surface* gCurrentSurface = NULL;
//Current displayed texture
SDL_Texture* gTexture = NULL;
//Globally used font
TTF_Font* gFont = NULL;
//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );
LTexture gTextTexture;
LTexture gTimeTextTexture;
LTexture gFooTexture;
LTexture gBackgroundTexture;
LTexture gButtonSpriteSheetTexture;
// SDL_Rect gSpriteClips[ 4 ];
SDL_Rect gButtonSpriteClips[ BUTTON_SPRITE_TOTAL ];
LButton gButtons[ TOTAL_BUTTONS ];
// LTexture gSpriteSheetTexture;
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];
LTexture gSpriteSheetTexture;
LTexture gFPSTextTexture;
LTexture gCharTexture;
// Character* character = NULL;


bool init()
{
    //Initialization flag
    bool success = true;

    // using namespace std;
    SDL_Init( SDL_INIT_EVERYTHING );

    if (auto error = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT,
         SDL_WINDOW_SHOWN | SDL_RENDERER_ACCELERATED , &gWindow, &gRenderer )) {
        printf("Error creating Window: %s\n", SDL_GetError());
        success = false;
         } else {
             //Initialize PNG loading
             // SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
             int imgFlags = IMG_INIT_PNG;
             if( !( IMG_Init( imgFlags ) & imgFlags ) )
             {
                 printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                 success = false;
             }
             //Initialize SDL_ttf
             else if( TTF_Init() == -1 )
             {
                 printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                 success = false;
             }
             else
             {
                 //Get window surface
                 gScreenSurface = SDL_GetWindowSurface( gWindow );
                 SDL_SetWindowTitle(gWindow, "Cowabunger");
             }


         }
    // character = new Character(gRenderer, gCharTexture);
    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    if( !gCharTexture.loadFromFile( "../assets/dot.bmp", gRenderer ) )
    {
    printf( "Failed to load dot texture!\n" );
    success = false;
    }else {
        printf("Successfully loaded dot texture.\n");
    }
    //Load sprites
    if( !gButtonSpriteSheetTexture.loadFromFile( "../assets/button.png", gRenderer ) )
    {
        printf( "Failed to load button sprite texture!\n" );
        success = false;
    }
    else
    {
        //Set sprites
        for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
        {
            gSpriteClips[ i ].x = 0;
            gSpriteClips[ i ].y = i * 200;
            gSpriteClips[ i ].w = BUTTON_WIDTH;
            gSpriteClips[ i ].h = BUTTON_HEIGHT;
        }

        //Set buttons in corners
        gButtons[ 0 ].setPosition( 0, 0 );
        gButtons[ 1 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, 0 );
        gButtons[ 2 ].setPosition( 0, SCREEN_HEIGHT - BUTTON_HEIGHT );
        gButtons[ 3 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT );
    }

    //Load sprite sheet texture
    if( !gSpriteSheetTexture.loadFromFile( "../assets/playWalkSprite.png" ,gRenderer) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        gSpriteClips[ 0 ].x =   0;
        gSpriteClips[ 0 ].y =   0;
        gSpriteClips[ 0 ].w =  64;
        gSpriteClips[ 0 ].h = 205;

        gSpriteClips[ 1 ].x =  64;
        gSpriteClips[ 1 ].y =   0;
        gSpriteClips[ 1 ].w =  64;
        gSpriteClips[ 1 ].h = 205;

        gSpriteClips[ 2 ].x = 128;
        gSpriteClips[ 2 ].y =   0;
        gSpriteClips[ 2 ].w =  64;
        gSpriteClips[ 2 ].h = 205;

        gSpriteClips[ 3 ].x = 192;
        gSpriteClips[ 3 ].y =   0;
        gSpriteClips[ 3 ].w =  64;
        gSpriteClips[ 3 ].h = 205;
    }
    //Open the font
    gFont = TTF_OpenFont( "../assets/woodyFont.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Render text
        SDL_Color textColor = { 0, 0, 0, 255 };
        if( !gTextTexture.loadFromRenderedText( "Cowabungah!", textColor ,gRenderer, gFont) )
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
        if( !gTimeTextTexture.loadFromRenderedText( "Game time:", textColor ,gRenderer, gFont) )
        {
            printf( "Failed to render time text texture!\n" );
            success = false;
        }
    }

    //Load Foo' texture
    if( !gFooTexture.loadFromFile( "../assets/playerIdle.png" ,gRenderer) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }

    //Load background texture
    if( !gBackgroundTexture.loadFromFile( "../assets/background_resized.png",gRenderer ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }

    //Load PNG texture
    gTexture = loadTexture( "../assets/cowRight.png" );
    if( gTexture == NULL )
    {
        printf( "Failed to load texture image!\n" );
        success = false;
    }

    //Load up surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface( "../assets/blackbuckUp.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL )
    {
        printf( "Failed to load up image!\n" );
        success = false;
    }

    //Load down surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "../assets/blackbuckDown.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL )
    {
        printf( "Failed to load down image!\n" );
        success = false;
    }

    //Load left surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface( "../assets/blackbuckLeft.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL )
    {
        printf( "Failed to load left image!\n" );
        success = false;
    }

    //Load right surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface( "../assets/blackbuckRight.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
    {
        printf( "Failed to load right image!\n" );
        success = false;
    }

    return success;
}


SDL_Surface* loadSurface( std::string path )
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}

SDL_Texture* loadTexture( std::string path )
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

void close()
{

    //Free loaded images
    gFooTexture.free();
    gBackgroundTexture.free();
    gFPSTextTexture.free();
    // gCharTexture->free();
    //Deallocate surface
    SDL_FreeSurface( goatImg );
    goatImg = NULL;

    //Free loaded image
    SDL_DestroyTexture( gTexture );
    gTexture = NULL;

    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}





int main(int argc, char* argv[]) {
    using namespace std;
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else {
            //Set default current surface
            //Apply the image
            //Current animation frame
            int frame = 0;
            SDL_Event e;
            LTexture* currentTexture = NULL;
            //Angle of rotation
            double degrees = 0;
            //Set text color as black
            SDL_Color textColor = { 0, 0, 0, 255 };
            //Current time start time
            Uint32 startTime = 0;
            //In memory text stream
            std::stringstream timeText;
            LTimer timer;
            //The frames per second timer
            LTimer fpsTimer;
            //The frames per second cap timer
            LTimer capTimer;
            //Start counting frames per second
            int countedFrames = 0;
            fpsTimer.start();

            //Flip type
            SDL_RendererFlip flipType = SDL_FLIP_NONE;
            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
            Character character(gRenderer, &gCharTexture);
            bool quit = false;
            while( !quit ) {
                //Start cap timer
                capTimer.start();

                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    else if( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_a:
                                degrees -= 60;
                            break;

                            case SDLK_d:
                                degrees += 60;
                            break;

                            case SDLK_q:
                                flipType = SDL_FLIP_HORIZONTAL;
                            break;

                            case SDLK_w:
                                flipType = SDL_FLIP_NONE;
                            break;

                            case SDLK_e:
                                flipType = SDL_FLIP_VERTICAL;
                            break;
                            case SDLK_s:
                                if( timer.isStarted() )
                                {
                                    timer.stop();
                                }
                                else
                                {
                                    timer.start();
                                }
                            break;
                            case SDLK_p:
                                if( timer.isPaused() )
                                {
                                    timer.unpause();
                                }
                                else
                                {
                                    timer.pause();
                                }
                            break;

                        }

                    }

                    //Handle input for the dot
                    character.handleEvent( e );
                    startTime = SDL_GetTicks();

                }
                character.move();
                //Set texture based on current keystate


                // SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                //Calculate and correct fps
                float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
                if( avgFPS > 20000)
                {
                    avgFPS = 0;
                }

                // Set text to be rendered
                timeText.str( "" );
                timeText << "Seconds since start time " << timer.getTicks() / 1000.f; // custom Timer
                //Render text
                if( !gTimeTextTexture.loadFromRenderedText( timeText.str(), textColor,gRenderer,gFont ) )
                {
                printf( "Unable to render time texture!\n" );
                }
                timeText.str( "" );
                timeText << "FPS " << avgFPS; // custom Timer
                //Render text
                if( !gFPSTextTexture.loadFromRenderedText( timeText.str(), textColor ,gRenderer, gFont) )
                {
                    printf( "Unable to render FPS texture!\n" );
                }
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );
                gBackgroundTexture.renderB( gRenderer,0, 0 );
                character.render();

                //Render background texture to screen
                gTimeTextTexture.renderB( gRenderer,(( SCREEN_WIDTH - gTextTexture.getWidth()) / 2), ( SCREEN_HEIGHT - gTextTexture.getHeight() ));

                //Render textures
                gFPSTextTexture.render(  gRenderer,0 , ( gFPSTextTexture.getHeight() )  );

                //Render current frame
                gTextTexture.render( gRenderer,( SCREEN_WIDTH - gTextTexture.getWidth() - 100) , (  gTextTexture.getHeight() ) );

                // gFooTexture.render( 240, 190 );
                //Update screen // Player
                SDL_Rect* currentClip = &gSpriteClips[ frame / 4 ];
                gSpriteSheetTexture.render( gRenderer,( SCREEN_WIDTH - currentClip->w ) / 2, ( SCREEN_HEIGHT - currentClip->h ) / 2, currentClip ,  degrees, NULL, flipType);



                //Update screen
                // SDL_RenderPresent( gRenderer );
                SDL_RenderPresent( gRenderer );
                //Go to next frame
                ++frame;
                ++countedFrames;

                //Cycle animation
                 if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                 {
                     frame = 0;
                 }

                //If frame finished early
                int frameTicks = capTimer.getTicks();
                if( frameTicks < SCREEN_TICKS_PER_FRAME )
                {
                    //Wait remaining time
                    SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
                }
            }

        }

        close();
        return 0;
    }
}