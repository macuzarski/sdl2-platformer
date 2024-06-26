//
// Created by macuz on 14.05.2024.
//

#include "defs.h"
#include <iostream>
#include <SDL.h>
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
            SDL_Event e;
            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
            bool quit = false;
            while( !quit ) {
                while( SDL_PollEvent( &e ) != 0 ) {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    //User presses a key
                    else if( e.type == SDL_KEYDOWN )
                    {
                        //Select surfaces based on key press
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_UP:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                            break;

                            case SDLK_DOWN:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                            break;

                            case SDLK_LEFT:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                            break;

                            case SDLK_RIGHT:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                            break;

                            default:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                            break;
                        }
                    }
                }
                SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );
                SDL_UpdateWindowSurface( gWindow );
            }

        }


        close();
        return 0;
    }
}

bool init()
{
    //Initialization flag
    bool success = true;

    // using namespace std;
    SDL_Init( SDL_INIT_EVERYTHING );

    if (auto error = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT,
         SDL_WINDOW_SHOWN, &gWindow, &gRenderer )) {
        printf("Error creating Window: %s\n", SDL_GetError());
        success = false;
         }
    SDL_SetWindowTitle(gWindow, "Cowabunger");

    gScreenSurface = SDL_GetWindowSurface(gWindow);

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load default surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "../assets/blackbuckRight.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
    {
        printf( "Failed to load default image!\n" );
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

void close()
{
    //Deallocate surface
    SDL_FreeSurface( goatImg );
    goatImg = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    SDL_DestroyRenderer( gRenderer );


    //Quit SDL subsystems
    SDL_Quit();
}

SDL_Surface* loadSurface( std::string path )
{
    //Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }

    return loadedSurface;
}