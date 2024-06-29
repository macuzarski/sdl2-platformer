//
// Created by macuz on 28.06.2024.
//
#include "Character.h"
#include "defs.h"
#include "LTexture.h"
bool onTopOfEnemy = false;
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
    mColliders.resize( 8 );

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
    mJumping = false;
    mJumpStartY = 0;
    mMoving = false;

    //Initialize the collision boxes' width and height
    mColliders[ 0 ].w = 80;
    mColliders[ 0 ].h = 7;

    mColliders[ 1 ].w = 80;
    mColliders[ 1 ].h = 7;

    mColliders[ 2 ].w = 80;
    mColliders[ 2 ].h = 7;

    mColliders[ 3 ].w = 80;
    mColliders[ 3 ].h = 7;

    mColliders[ 4 ].w = 80;
    mColliders[ 4 ].h = 7;

    mColliders[ 5 ].w = 80;
    mColliders[ 5 ].h = 7;

    mColliders[ 6 ].w = 80;
    mColliders[ 6 ].h = 5;

    mColliders[ 7 ].w = 80;
    mColliders[ 7 ].h = 5;

    // mColliders[ 8 ].w = 14;
    // mColliders[ 8 ].h = 1;
    //
    // mColliders[ 9 ].w = 14;
    // mColliders[ 9 ].h = 1;
    //
    // mColliders[ 10 ].w = 14;
    // mColliders[ 10 ].h = 1;

    //Initialize colliders relative to position
    shiftColliders();

    renderer = render;
    gCharacterTexture = texture;
    mClips = clips;
    mNumClips = numClips;
    mCurrentClipIndex = 0;

}

void Character::move(std::vector<SDL_Rect>& otherColliders)
{
    printf("on enemy (%d)\n", onTopOfEnemy);
    // Move the character left or right
    mPosX += mVelX;
    shiftColliders();

    // If the character went too far to the left or right
    if ((mPosX < 0) || (mPosX + CHARACTER_WIDTH > LEVEL_WIDTH) || checkCollision(mColliders, otherColliders) && !onTopOfEnemy) {
        mPosX -= mVelX;
        shiftColliders();
    }

    // Apply gravity continuously
    if (!mJumping) {
        mVelY += GRAVITY;
    }

    // Check for collision with enemy or ground
    if (checkCollision(mColliders, otherColliders) || mPosY + CHARACTER_HEIGHT >= GROUND_LEVEL) {
        if (mVelY > 0) { // Only stop falling if moving down
            mJumping = false;
            mVelY = 0;
        }
    } else {
        mPosY += mVelY;
        shiftColliders();
    }

    // Jump move
    if (mJumping) {
        mPosY += mVelY;
        mVelY += GRAVITY;

        if (checkCollision(mColliders, otherColliders)) {
            mJumping = false;
            mVelY = 0;
        }
        else if (mPosY >= mJumpStartY) {
            mPosY = mJumpStartY;
            mJumping = false;
            mVelY = 0;
        }
    }

    // Move the character up or down
    mPosY += mVelY;
    shiftColliders();

    // If the character went too far up or down
    if ((mPosY < 0) || (mPosY  > GROUND_LEVEL) || checkCollision(mColliders, otherColliders)) {
        mPosY -= mVelY;
        shiftColliders();
    }

    // Check if player is on top of an enemy
    for (auto& enemyCollider : otherColliders) {
        if (mPosY + CHARACTER_HEIGHT <= enemyCollider.y && mPosY + CHARACTER_HEIGHT + mVelY > enemyCollider.y &&
            mPosX + CHARACTER_WIDTH > enemyCollider.x && mPosX < enemyCollider.x + enemyCollider.w) {
            mPosY = enemyCollider.y - CHARACTER_HEIGHT;
            mJumping = false;
            mVelY = 0;
            onTopOfEnemy = true;
            break;
        }
    }

    // Check if player has moved off the enemy
    if (onTopOfEnemy) {
        bool stillOnEnemy = false;
        for (auto& enemyCollider : otherColliders) {
            if (mPosX + CHARACTER_WIDTH > enemyCollider.x && mPosX < enemyCollider.x + enemyCollider.w) {
                stillOnEnemy = true;
                break;
            }
        }
        if (!stillOnEnemy) {
            onTopOfEnemy=false;
            mVelY = GRAVITY; // Start falling down
        }
        printf("still on enemy (%d)\n", stillOnEnemy);
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

Player::Player(SDL_Renderer* render, LTexture* texture, SDL_Rect* clips, int numClips, int x, int y)
    : Character(render, texture, clips, numClips, x, y), mFlip(SDL_FLIP_NONE) {}

void Player::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        mMoving = true;
        switch( e.key.keysym.sym )
        {
            case SDLK_UP:
                if (!mJumping) {
                    mVelY = -JUMP_VELOCITY;
                    mJumping = true;
                    mJumpStartY = mPosY;
                }
            break;
            case SDLK_LEFT: mVelX -= CHARACTER_VEL; mFlip=SDL_FLIP_HORIZONTAL; break;
            case SDLK_RIGHT: mVelX += CHARACTER_VEL; mFlip=SDL_FLIP_NONE; break;
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

void Player::render() {
    if (!mMoving) {
        gCharacterTexture->render(renderer, mPosX, mPosY, &mClips[2], 0.0, nullptr, mFlip);
    } else {
        gCharacterTexture->render(renderer, mPosX, mPosY, &mClips[mCurrentClipIndex], 0.0, nullptr, mFlip);
    }
}

void Player::render(int camX, int camY) {
    if (!mMoving) {
        gCharacterTexture->render(renderer, mPosX - camX, mPosY - camY, &mClips[2], 0.0, nullptr, mFlip);
    } else {
        gCharacterTexture->render(renderer, mPosX - camX, mPosY - camY, &mClips[mCurrentClipIndex], 0.0, nullptr, mFlip);
    }
}

Enemy::Enemy(SDL_Renderer* render, LTexture* texture, SDL_Rect* clips, int numClips, int x, int y)
    : Character(render, texture, clips, numClips, x, y) {}

void Enemy::render() {
    gCharacterTexture->render(renderer, mPosX, mPosY, &mClips[mCurrentClipIndex]);
}

void Enemy::render(int camX, int camY) {
    gCharacterTexture->render(renderer, mPosX - camX, mPosY - camY, &mClips[mCurrentClipIndex]+7, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
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

