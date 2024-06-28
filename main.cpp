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



class LTexture
{
public:
    //Initializes variables
    explicit LTexture();

    //Deallocates memory
    ~LTexture();

    //Loads image at specified path
    bool loadFromFile( std::string path );
    //Creates image from font string
    bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

    //Deallocates texture
    void free();

    //Set color modulation
    void setColor( Uint8 red, Uint8 green, Uint8 blue );

    //Set blending
    void setBlendMode( SDL_BlendMode blending );

    //Set alpha modulation
    void setAlpha( Uint8 alpha );

    //Renders texture at given point
    void renderB( int x, int y);
    void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
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

//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};
//The mouse button
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
    void render();

private:
    //Top left position
    SDL_Point mPosition;

    //Currently used global sprite
    LButtonSprite mCurrentSprite;
};
//The application time based timer
class LTimer
{
public:
    //Initializes variables
    LTimer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    Uint32 getTicks();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();

private:
    //The clock time when the timer started
    Uint32 mStartTicks;

    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;
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

LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}
void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
    //Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
    //Timer is running and paused
    return mPaused && mStarted;
}


LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
    mPosition.x = x;
    mPosition.y = y;
}
void LButton::handleEvent( SDL_Event* e )
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if( x < mPosition.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > mPosition.x + BUTTON_WIDTH )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < mPosition.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > mPosition.y + BUTTON_HEIGHT )
        {
            inside = false;
        }
        //Mouse is outside button
        if( !inside )
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

                case SDL_MOUSEBUTTONDOWN:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;

                case SDL_MOUSEBUTTONUP:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
    }
}


void LButton::render()
{
    //Show current button sprite
    gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}


LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}
LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::loadFromFile( std::string path )
{
    //Get rid of preexisting texture
    free();

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
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
    return mTexture != NULL;
}

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}
void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}
void LTexture::setBlendMode( SDL_BlendMode blending )
{
    //Set blending function
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( mTexture, alpha );
}


void LTexture::renderB( int x, int y )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
}
void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}



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
    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;
    //Load sprite sheet texture
    // if( !gSpriteSheetTexture.loadFromFile( "../assets/dots.png" ) )
    // {
    //     printf( "Failed to load sprite sheet texture!\n" );
    //     success = false;
    // }
    // else
    // {
    //     //Set top left sprite
    //     gSpriteClips[ 0 ].x =   0;
    //     gSpriteClips[ 0 ].y =   0;
    //     gSpriteClips[ 0 ].w = 100;
    //     gSpriteClips[ 0 ].h = 100;
    //
    //     //Set top right sprite
    //     gSpriteClips[ 1 ].x = 100;
    //     gSpriteClips[ 1 ].y =   0;
    //     gSpriteClips[ 1 ].w = 100;
    //     gSpriteClips[ 1 ].h = 100;
    //
    //     //Set bottom left sprite
    //     gSpriteClips[ 2 ].x =   0;
    //     gSpriteClips[ 2 ].y = 100;
    //     gSpriteClips[ 2 ].w = 100;
    //     gSpriteClips[ 2 ].h = 100;
    //
    //     //Set bottom right sprite
    //     gSpriteClips[ 3 ].x = 100;
    //     gSpriteClips[ 3 ].y = 100;
    //     gSpriteClips[ 3 ].w = 100;
    //     gSpriteClips[ 3 ].h = 100;
    // }

    //Load sprites
    if( !gButtonSpriteSheetTexture.loadFromFile( "../assets/button.png" ) )
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
    if( !gSpriteSheetTexture.loadFromFile( "../assets/playWalkSprite.png" ) )
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
        if( !gTextTexture.loadFromRenderedText( "Cowabungah!", textColor ) )
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
        if( !gTimeTextTexture.loadFromRenderedText( "Game time:", textColor ) )
        {
            printf( "Failed to render time text texture!\n" );
            success = false;
        }
    }

    //Load Foo' texture
    if( !gFooTexture.loadFromFile( "../assets/playerIdle.png" ) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }

    //Load background texture
    if( !gBackgroundTexture.loadFromFile( "../assets/background_resized.png" ) )
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

    //Load default surface
    // gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "../assets/blackbuckRight.bmp" );
    // if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
    // {
    //     printf( "Failed to load default image!\n" );
    //     success = false;
    // }

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
            bool quit = false;
            while( !quit ) {
                //Start cap timer
                capTimer.start();
                // while( SDL_PollEvent( &e ) != 0 ) {
                //     //User requests quit
                //     if( e.type == SDL_QUIT )
                //     {
                //         quit = true;
                //     }
                //     //User presses a key
                //     else if( e.type == SDL_KEYDOWN )
                //     {
                //         // cout << "event: " << "dsa";
                //         //Select surfaces based on key press
                //         switch( e.key.keysym.sym )
                //         {
                //             case SDLK_UP:
                //                 cout << "event: " << "dsa";
                //                 gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                //             break;
                //
                //             case SDLK_DOWN:
                //                 gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                //             break;
                //
                //             case SDLK_LEFT:
                //                 gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                //             break;
                //
                //             case SDLK_RIGHT:
                //                 gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                //             break;
                //
                //             default:
                //                 gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                //             break;
                //         }
                //     }
                // }
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    // else if( e.type == SDL_KEYDOWN )
                    // {
                    //     switch( e.key.keysym.sym )
                    //     {
                    //         case SDLK_a:
                    //             degrees -= 60;
                    //         break;
                    //
                    //         case SDLK_d:
                    //             degrees += 60;
                    //         break;
                    //
                    //         case SDLK_q:
                    //             flipType = SDL_FLIP_HORIZONTAL;
                    //         break;
                    //
                    //         case SDLK_w:
                    //             flipType = SDL_FLIP_NONE;
                    //         break;
                    //
                    //         case SDLK_e:
                    //             flipType = SDL_FLIP_VERTICAL;
                    //         break;
                    //     }
                    //
                    // }
                    else if( e.type == SDL_KEYDOWN )
                    {
                        //Start/stop
                        if( e.key.keysym.sym == SDLK_s )
                        {
                            if( timer.isStarted() )
                            {
                                timer.stop();
                            }
                            else
                            {
                                timer.start();
                            }
                        }
                        //Pause/unpause
                        else if( e.key.keysym.sym == SDLK_p )
                        {
                            if( timer.isPaused() )
                            {
                                timer.unpause();
                            }
                            else
                            {
                                timer.pause();
                            }
                        }
                    }
                    startTime = SDL_GetTicks();



                }
                //Set texture based on current keystate
                // const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
                // if( currentKeyStates[ SDL_SCANCODE_UP ] )
                // {
                //     currentTexture = &gUpTexture;
                // }
                // else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
                // {
                //     currentTexture = &gDownTexture;
                // }
                // else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
                // {
                //     currentTexture = &gLeftTexture;
                // }
                // else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
                // {
                //     currentTexture = &gRightTexture;
                // }
                // else
                // {
                //     currentTexture = &gPressTexture;
                // }

                // SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                //Calculate and correct fps
                float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
                if( avgFPS > 20000)
                {
                    avgFPS = 0;
                }
                // cout << "fps: " << avgFPS;

                // Set text to be rendered
                timeText.str( "" );
                // timeText << "Milliseconds since start time " << SDL_GetTicks() - startTime; // Simple SDL based timer
                timeText << "Seconds since start time " << timer.getTicks() / 1000.f; // custom Timer
                //Render text
                if( !gTimeTextTexture.loadFromRenderedText( timeText.str(), textColor ) )
                {
                printf( "Unable to render time texture!\n" );
                }
                timeText.str( "" );
                timeText << "FPS " << avgFPS; // custom Timer
                //Render text
                if( !gFPSTextTexture.loadFromRenderedText( timeText.str(), textColor ) )
                {
                    printf( "Unable to render FPS texture!\n" );
                }
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );
                //Apply the image stretched
                // SDL_Rect stretchRect;
                // stretchRect.x = 0;
                // stretchRect.y = 0;
                // stretchRect.w = SCREEN_WIDTH;
                // stretchRect.h = SCREEN_HEIGHT;
                // SDL_BlitScaled( gCurrentSurface, NULL, gScreenSurface, &stretchRect );
                //Render background texture to screen
                gBackgroundTexture.renderB( 0, 0 );
                gTimeTextTexture.renderB( (( SCREEN_WIDTH - gTextTexture.getWidth()) / 2), ( SCREEN_HEIGHT - gTextTexture.getHeight() ));

                //Render textures
                gFPSTextTexture.render(  0 , ( gFPSTextTexture.getHeight() )  );

                //Render current frame
                gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() - 100) , (  gTextTexture.getHeight() ) );

                // gFooTexture.render( 240, 190 );
                //Update screen // Player
                SDL_Rect* currentClip = &gSpriteClips[ frame / 4 ];
                gSpriteSheetTexture.render( ( SCREEN_WIDTH - currentClip->w ) / 2, ( SCREEN_HEIGHT - currentClip->h ) / 2, currentClip ,  degrees, NULL, flipType);

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

                // //Clear screen
                // SDL_RenderClear( gRenderer );
                // //Render top left sprite
                // gSpriteSheetTexture.render( 0, 0, &gSpriteClips[ 0 ] );
                //
                // //Render top right sprite
                // gSpriteSheetTexture.render( SCREEN_WIDTH - gSpriteClips[ 1 ].w, 0, &gSpriteClips[ 1 ] );
                //
                // //Render bottom left sprite
                // gSpriteSheetTexture.render( 0, SCREEN_HEIGHT - gSpriteClips[ 2 ].h, &gSpriteClips[ 2 ] );
                //
                // //Render bottom right sprite
                // gSpriteSheetTexture.render( SCREEN_WIDTH - gSpriteClips[ 3 ].w, SCREEN_HEIGHT - gSpriteClips[ 3 ].h, &gSpriteClips[ 3 ] );
                // //Update screen
                // SDL_RenderPresent( gRenderer );

                // SDL_Rect topLeftViewport;
                // topLeftViewport.x = 0;
                // topLeftViewport.y = 0;
                // topLeftViewport.w = SCREEN_WIDTH / 2;
                // topLeftViewport.h = SCREEN_HEIGHT / 2;
                // SDL_RenderSetViewport( gRenderer, &topLeftViewport );
                // //Render texture to screen
                // SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
                // //Top right viewport
                // SDL_Rect topRightViewport;
                // topRightViewport.x = SCREEN_WIDTH / 2;
                // topRightViewport.y = 0;
                // topRightViewport.w = SCREEN_WIDTH / 2;
                // topRightViewport.h = SCREEN_HEIGHT / 2;
                // SDL_RenderSetViewport( gRenderer, &topRightViewport );
                // SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
                //
                // //Bottom viewport
                // SDL_Rect bottomViewport;
                // bottomViewport.x = 0;
                // bottomViewport.y = SCREEN_HEIGHT / 2;
                // bottomViewport.w = SCREEN_WIDTH;
                // bottomViewport.h = SCREEN_HEIGHT / 2;
                // SDL_RenderSetViewport( gRenderer, &bottomViewport );
                //
                // //Render texture to screen
                // SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
                //
                //
                // //Update screen
                // SDL_RenderPresent( gRenderer );
                //
                //
                // //Render texture to screen
                // SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
                //
                // //Update screen
                // SDL_RenderPresent( gRenderer );
                //
                // // SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );
                // SDL_UpdateWindowSurface( gWindow );
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