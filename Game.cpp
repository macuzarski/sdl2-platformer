//
// Created by macuz on 29.06.2024.
//
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


enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};


SDL_Surface *loadSurface(std::string path);

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
//The surface contained by the window
SDL_Surface *gScreenSurface = NULL;
//The image we will load and show on the screen
SDL_Surface *goatImg = NULL;
//The images that correspond to a keypress
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
//Current displayed image
SDL_Surface *gCurrentSurface = NULL;
//Current displayed texture
SDL_Texture *gTexture = NULL;
//Globally used font
TTF_Font *gFont = NULL;

//Loads individual image as texture
SDL_Texture *loadTexture(std::string path);

LTexture gTextTexture;
LTexture gTimeTextTexture;
LTexture gFooTexture;
LTexture gBackgroundTexture;
LTexture gButtonSpriteSheetTexture;
// SDL_Rect gSpriteClips[ 4 ];
SDL_Rect gButtonSpriteClips[BUTTON_SPRITE_TOTAL];
LButton gButtons[TOTAL_BUTTONS];
// LTexture gSpriteSheetTexture;
const int WALKING_ANIMATION_FRAMES = 7;
SDL_Rect gToroClips[WALKING_ANIMATION_FRAMES];
LTexture gToroAnimated;;
LTexture gFPSTextTexture;
LTexture gCharTexture;
LTexture gBGTexture;


bool init() {
    //Initialization flag
    bool success = true;

    // using namespace std;
    SDL_Init(SDL_INIT_EVERYTHING);

    if (auto error = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT,
                                                 SDL_WINDOW_SHOWN | SDL_RENDERER_ACCELERATED, &gWindow, &gRenderer)) {
        printf("Error creating Window: %s\n", SDL_GetError());
        success = false;
    } else {
        //Initialize PNG loading
        // SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
            success = false;
        }
        //Initialize SDL_ttf
        else if (TTF_Init() == -1) {
            printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
            success = false;
        } else {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
            SDL_SetWindowTitle(gWindow, "Cowabunger");
        }
    }
    // character = new Character(gRenderer, gCharTexture);
    return success;
}

bool loadMedia() {
    //Loading success flag
    bool success = true;

    if (!gCharTexture.loadFromFile("../assets/dot.bmp", gRenderer)) {
        printf("Failed to load dot texture!\n");
        success = false;
    } else {
        printf("Successfully loaded dot texture.\n");
    }
    //Load sprites
    if (!gButtonSpriteSheetTexture.loadFromFile("../assets/button.png", gRenderer)) {
        printf("Failed to load button sprite texture!\n");
        success = false;
    }
    //Load background texture
    if (!gBGTexture.loadFromFile("..//assets/mapv3.png", gRenderer)) {
        printf("Failed to load background texture!\n");
        success = false;
    } else {
        //Set sprites
        for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i) {
            // gSpriteClips[ i ].x = 0;
            // gSpriteClips[ i ].y = i * 200;
            // gSpriteClips[ i ].w = BUTTON_WIDTH;
            // gSpriteClips[ i ].h = BUTTON_HEIGHT;
        }

        //Set buttons in corners
        gButtons[0].setPosition(0, 0);
        gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
        gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
        gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
    }

    // Load sprite sheet texture
     if (!gToroAnimated.loadFromFile("../assets/toro.png", gRenderer)) {
         printf("Failed to load walking animation texture!\n");
         success = false;
     } else {
         //Set sprite clips
         gToroClips[0].x = 0;
         gToroClips[0].y = 0;
         gToroClips[0].w = 111;
         gToroClips[0].h = 81;

         gToroClips[1].x = 111;
         gToroClips[1].y = 0;
         gToroClips[1].w = 111;
         gToroClips[1].h = 81;

         gToroClips[2].x = 222;
         gToroClips[2].y = 0;
         gToroClips[2].w = 111;
         gToroClips[2].h = 81;

         gToroClips[3].x = 333;
         gToroClips[3].y = 0;
         gToroClips[3].w = 111;
         gToroClips[3].h = 81;

         gToroClips[4].x = 444;
         gToroClips[4].y = 0;
         gToroClips[4].w = 111;
         gToroClips[4].h = 81;

         gToroClips[5].x = 555;
         gToroClips[5].y = 0;
         gToroClips[5].w = 111;
         gToroClips[5].h = 81;

         gToroClips[6].x = 666;
         gToroClips[6].y = 0;
         gToroClips[6].w = 111;
         gToroClips[6].h = 81;

        // gToroClips[7].x = 0;
        // gToroClips[7].y = 81;
        // gToroClips[7].w = 111;
        // gToroClips[7].h = 81;
        //
        // gToroClips[8].x = 111;
        // gToroClips[8].y = 81;
        // gToroClips[8].w = 111;
        // gToroClips[8].h = 81;
        //
        // gToroClips[9].x = 222;
        // gToroClips[9].y = 81;
        // gToroClips[9].w = 111;
        // gToroClips[9].h = 81;
        //
        // gToroClips[10].x = 333;
        // gToroClips[10].y = 81;
        // gToroClips[10].w = 111;
        // gToroClips[10].h = 81;
        //
        // gToroClips[11].x = 444;
        // gToroClips[11].y = 81;
        // gToroClips[11].w = 111;
        // gToroClips[11].h = 81;
        //
        // gToroClips[12].x = 555;
        // gToroClips[12].y = 81;
        // gToroClips[12].w = 111;
        // gToroClips[12].h = 81;
        //
        // gToroClips[13].x = 666;
        // gToroClips[13].y = 81;
        // gToroClips[13].w = 111;
        // gToroClips[13].h = 81;
        //
        // gToroClips[14].x = 0;
        // gToroClips[14].y = 162;
        // gToroClips[14].w = 111;
        // gToroClips[14].h = 81;
        //
        // gToroClips[15].x = 111;
        // gToroClips[15].y = 162;
        // gToroClips[15].w = 111;
        // gToroClips[15].h = 81;
        //
        // gToroClips[16].x = 222;
        // gToroClips[16].y = 162;
        // gToroClips[16].w = 111;
        // gToroClips[16].h = 81;
        //
        // gToroClips[17].x = 333;
        // gToroClips[17].y = 162;
        // gToroClips[17].w = 111;
        // gToroClips[17].h = 81;
    }
    //Open the font
    gFont = TTF_OpenFont("../assets/woodyFont.ttf", 28);
    if (gFont == NULL) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    } else {
        //Render text
        SDL_Color textColor = {0, 0, 0, 255};
        if (!gTextTexture.loadFromRenderedText("Cowabungah!", textColor, gRenderer, gFont)) {
            printf("Failed to render text texture!\n");
            success = false;
        }
        if (!gTimeTextTexture.loadFromRenderedText("Game time:", textColor, gRenderer, gFont)) {
            printf("Failed to render time text texture!\n");
            success = false;
        }
    }

    //Load background texture
    if (!gBackgroundTexture.loadFromFile("../assets/map.bmp", gRenderer)) {
        printf("Failed to load background texture image!\n");
        success = false;
    }

    //Load PNG texture
    gTexture = loadTexture("../assets/cowRight.png");
    if (gTexture == NULL) {
        printf("Failed to load texture image!\n");
        success = false;
    }
    return success;

}


SDL_Surface *loadSurface(std::string path) {
    //The final optimized image
    SDL_Surface *optimizedSurface = NULL;
    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    } else {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == NULL) {
            printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

SDL_Texture *loadTexture(std::string path) {
    //The final texture
    SDL_Texture *newTexture = NULL;

    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void close() {
    //Free loaded images
    gFooTexture.free();
    gBackgroundTexture.free();
    gFPSTextTexture.free();
    gBGTexture.free();
    // gToroAnimated.free();
    // gCharTexture->free();
    gCharTexture.free();
    //Deallocate surface
    SDL_FreeSurface(goatImg);
    goatImg = NULL;

    //Free loaded image
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    //Free global font
    TTF_CloseFont(gFont);
    gFont = NULL;

    //Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


void run() {
    using namespace std;
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        //Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            int frame = 0;
            SDL_Event e;
            LTexture *currentTexture = NULL;
            //Angle of rotation
            double degrees = 0;
            //Set text color as black
            SDL_Color textColor = {0, 0, 0, 255};
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
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
            Character character(gRenderer, &gToroAnimated, gToroClips,7, 0, GROUND_FLOOR);
            // Character character(gRenderer, &gToroAnimated, 0, GROUND_FLOOR);
            Character enemy(gRenderer, &gToroAnimated, gToroClips,7, 250, GROUND_FLOOR);
            // Character enemy(gRenderer, &gCharTexture, 600, GROUND_FLOOR);
            SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            //The background scrolling offset
            // int scrollingOffset = 0;
            bool quit = false;
            timer.start();
            while (!quit) {
                //Start cap timer
                capTimer.start();

                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                            case SDLK_a: degrees -= 60;
                                break;

                            case SDLK_d: degrees += 60;
                                break;

                            case SDLK_q: flipType = SDL_FLIP_HORIZONTAL;
                                break;

                            case SDLK_w: flipType = SDL_FLIP_NONE;
                                break;

                            case SDLK_e: flipType = SDL_FLIP_VERTICAL;
                                break;

                            case SDLK_s:
                                // if( timer.isStarted() ){timer.stop();}else{timer.start();}
                                (timer.isStarted()) ? timer.stop() : timer.start();
                                break;
                            case SDLK_p:
                                (timer.isPaused()) ? timer.unpause() : timer.pause();
                                break;
                        }
                    }

                    //Handle input for the dot
                    character.handleEvent(e);
                    startTime = SDL_GetTicks();
                }

                character.move(enemy.getColliders());
                character.setCurrentClip(frame);
                //Center the camera over the dot
                camera.x = (character.getPosX() + Character::CHARACTER_WIDTH / 2) - SCREEN_WIDTH / 2;
                camera.y = (character.getPosY() + Character::CHARACTER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

                // SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                //Calculate and correct fps
                float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
                if (avgFPS > 20000) {
                    avgFPS = 0;
                }

                // Set text to be rendered
                timeText.str("");
                timeText << "Seconds since start time " << timer.getTicks() / 1000.f; // custom Timer
                //Render text
                if (!gTimeTextTexture.loadFromRenderedText(timeText.str(), textColor, gRenderer, gFont)) {
                    printf("Unable to render time texture!\n");
                }
                timeText.str("");
                timeText << "FPS " << avgFPS; // custom Timer
                //Render text
                if (!gFPSTextTexture.loadFromRenderedText(timeText.str(), textColor, gRenderer, gFont)) {
                    printf("Unable to render FPS texture!\n");
                }
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                //Keep the camera in bounds
                if (camera.x < 0) {
                    camera.x = 0;
                }
                if (camera.y < 0) {
                    camera.y = 0;
                }
                if (camera.x > LEVEL_WIDTH - camera.w) {
                    camera.x = LEVEL_WIDTH - camera.w;
                }
                if (camera.y > LEVEL_HEIGHT - camera.h) {
                    camera.y = LEVEL_HEIGHT - camera.h;
                }
                SDL_RenderClear(gRenderer);
                gBGTexture.render(gRenderer, 0, 0, &camera);
                character.render(camera.x, camera.y);
                enemy.render(camera.x, camera.y);

                //Render background texture to screen
                gTimeTextTexture.renderB(gRenderer, ((SCREEN_WIDTH - gTextTexture.getWidth()) / 2),
                                         (SCREEN_HEIGHT - gTextTexture.getHeight()));

                //Render textures
                gFPSTextTexture.render(gRenderer, 0, (gFPSTextTexture.getHeight()));

                //Render current frame
                gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth() - 100),
                                    (gTextTexture.getHeight()));

                // gFooTexture.render( 240, 190 );
                //Update screen // Player
                // SDL_Rect *currentClip = &gToroClips[frame / 4];
                // gToroAnimated.render(gRenderer, (SCREEN_WIDTH - currentClip->w) / 2,
                // (SCREEN_HEIGHT - currentClip->h) / 2, currentClip, degrees, NULL, flipType);


                //Update screen
                // SDL_RenderPresent( gRenderer );
                SDL_RenderPresent(gRenderer);
                //Go to next frame
                ++countedFrames;
                ++frame;

                // Cycle animation
                if (frame / 4 >= WALKING_ANIMATION_FRAMES) {
                frame = 0;
                }

                //If frame finished early
                int frameTicks = capTimer.getTicks();
                if (frameTicks < SCREEN_TICKS_PER_FRAME) {
                    //Wait remaining time
                    SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
                }
            }
        }

        close();
    }
}
