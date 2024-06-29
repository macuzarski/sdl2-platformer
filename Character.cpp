//
// Created by macuz on 28.06.2024.
//
#include "Character.h"
#include "defs.h"
#include "LTexture.h"
SDL_RendererFlip flip;

bool checkCollision( std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Go through the A boxes
    for( int Abox = 0; Abox < a.size(); Abox++ )
    {
        //Calculate the sides of rect A
        leftA = a[ Abox ].x;
        rightA = a[ Abox ].x + a[ Abox ].w;
        topA = a[ Abox ].y;
        bottomA = a[ Abox ].y + a[ Abox ].h;
        //Go through the B boxes
        for( int Bbox = 0; Bbox < b.size(); Bbox++ )
        {
            //Calculate the sides of rect B
            leftB = b[ Bbox ].x;
            rightB = b[ Bbox ].x + b[ Bbox ].w;
            topB = b[ Bbox ].y;
            bottomB = b[ Bbox ].y + b[ Bbox ].h;

            //If no sides from A are outside of B
            if( ( ( bottomA <= topB ) || ( topA >= bottomB ) || ( rightA <= leftB ) || ( leftA >= rightB ) ) == false )
            {
                //A collision is detected
                return true;
            }
        }
    }

    //If neither set of collision boxes touched
    return false;
}

// Character::Character(SDL_Renderer* render, LTexture* texture, int x, int y ) {
Character::Character(SDL_Renderer* render, LTexture* texture,SDL_Rect* clips, int numClips, int x, int y ) {
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

    //Create the necessary SDL_Rects
    mColliders.resize( 11 );

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
    mJumping = false;
    mJumpStartY = 0;
    mMoving = false;

    //Initialize the collision boxes' width and height
    mColliders[ 0 ].w = 6;
    mColliders[ 0 ].h = 1;

    mColliders[ 1 ].w = 10;
    mColliders[ 1 ].h = 1;

    mColliders[ 2 ].w = 14;
    mColliders[ 2 ].h = 1;

    mColliders[ 3 ].w = 16;
    mColliders[ 3 ].h = 2;

    mColliders[ 4 ].w = 18;
    mColliders[ 4 ].h = 2;

    mColliders[ 5 ].w = 20;
    mColliders[ 5 ].h = 6;

    mColliders[ 6 ].w = 18;
    mColliders[ 6 ].h = 2;

    mColliders[ 7 ].w = 16;
    mColliders[ 7 ].h = 2;

    mColliders[ 8 ].w = 14;
    mColliders[ 8 ].h = 1;

    mColliders[ 9 ].w = 10;
    mColliders[ 9 ].h = 1;

    mColliders[ 10 ].w = 6;
    mColliders[ 10 ].h = 1;

    //Initialize colliders relative to position
    shiftColliders();

    renderer = render;
    gCharacterTexture = texture;
    mClips = clips;
    mNumClips = numClips;
    mCurrentClipIndex = 0;
    mFlip = SDL_FLIP_NONE;
}

void Character::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        mMoving = true;
        switch( e.key.keysym.sym )
        {
            // case SDLK_UP: mVelY -= CHARACTER_VEL; break;
            case SDLK_UP:
                if (!mJumping) {
                    mVelY = -JUMP_VELOCITY;
                    mJumping = true;
                    mJumpStartY = mPosY;
                }
            break;
            case SDLK_DOWN: mVelY += CHARACTER_VEL; break;
            case SDLK_LEFT: mVelX -= CHARACTER_VEL; flip=SDL_FLIP_HORIZONTAL; break;
            case SDLK_RIGHT: mVelX += CHARACTER_VEL; flip=SDL_FLIP_NONE; break;
        }
    }    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {

            case SDLK_LEFT: mVelX += CHARACTER_VEL; mMoving = false;break;
            case SDLK_RIGHT: mVelX -= CHARACTER_VEL; mMoving = false;break;
        }
    }
}
void Character::move(std::vector<SDL_Rect>& otherColliders)
{
    //Move the character left or right
    mPosX += mVelX;
    shiftColliders();

    //If the character went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + CHARACTER_WIDTH > LEVEL_WIDTH )|| checkCollision( mColliders, otherColliders ) )
    {
        //Move back
        mPosX -= mVelX;
        shiftColliders();
    }

    //Jump move new
    if (mJumping) {
        mPosY += mVelY;
        mVelY += GRAVITY;

        if (mPosY >= mJumpStartY) {
            mPosY = mJumpStartY;
            mJumping = false;
            mVelY = 0;
        }
    }

    //Move the character up or down
    mPosY += mVelY;
    shiftColliders();

    //If the character went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + CHARACTER_HEIGHT > GROUND_LEVEL ) || checkCollision( mColliders, otherColliders ))
    {
        //Move back
        mPosY -= mVelY;
        shiftColliders();
    }
}

void Character::shiftColliders()
{
    //The row offset
    int r = 0;

    //Go through the dot's collision boxes
    for( int set = 0; set < mColliders.size(); ++set )
    {
        //Center the collision box
        mColliders[ set ].x = mPosX + ( CHARACTER_WIDTH - mColliders[ set ].w ) / 2;

        //Set the collision box at its row offset
        mColliders[ set ].y = mPosY + r;

        //Move the row offset down the height of the collision box
        r += mColliders[ set ].h;
    }
}



void Character::render()
{
    //Show the character
    // gCharacterTexture->renderB( renderer, mPosX, mPosY );
    gCharacterTexture->render( renderer, mPosX, mPosY, &mClips[mCurrentClipIndex] );
    // printf("Rendering character at position (%d, %d)\n", mPosX, mPosY);

}void Character::render(int camX, int camY)
{
    //Show the character
    if (mMoving == false) {
        gCharacterTexture->render( renderer, mPosX - camX, mPosY - camY, &mClips[2],0.0,nullptr, flip);
    } else {
        gCharacterTexture->render( renderer, mPosX - camX, mPosY - camY, &mClips[mCurrentClipIndex], 0.0, nullptr, flip);
    }
    // std::cout << gCharacterTexture.getTexturePath();
    // printf("Rendering character at position (%d, %d)\n", mPosX, mPosY);
}

void Character::setCurrentClip(int frame) {
    mCurrentClipIndex = frame / 4 % mNumClips;
}

std::vector<SDL_Rect>& Character::getColliders()
{
    return mColliders;
}

int Character::getPosX()
{
    return mPosX;
}

int Character::getPosY()
{
    return mPosY;
}

