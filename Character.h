#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include <vector>
#include "LTexture.h"

class Character {
public:
    static const int CHARACTER_WIDTH = 111;
    static const int CHARACTER_HEIGHT = 81;
    static const int CHARACTER_VEL = 10;
    static const int JUMP_VELOCITY = 15;
    static const int GRAVITY = 1;

    Character(SDL_Renderer* render, LTexture* texture, SDL_Rect* clips, int numClips, int x, int y);

    // void handleEvent(SDL_Event& e);
    void move(std::vector<SDL_Rect>& otherColliders);
    virtual void render() = 0;
    virtual void render(int camX, int camY) = 0;

    std::vector<SDL_Rect>& getColliders();
    int getPosX();
    int getPosY();
    void setCurrentClip(int frame);

protected:
    int mPosX, mPosY;
    int mVelX, mVelY;
    bool mJumping;
    int mJumpStartY;
    bool mMoving;
    SDL_Renderer* renderer;
    LTexture* gCharacterTexture;
    SDL_Rect* mClips;
    int mNumClips;
    int mCurrentClipIndex;
    std::vector<SDL_Rect> mColliders;

    void shiftColliders();
};

class Player : public Character {
public:
    Player(SDL_Renderer* render, LTexture* texture, SDL_Rect* clips, int numClips, int x, int y);

    void handleEvent(SDL_Event& e);

    void render() override;

    void render(int camX, int camY) override;

private:
    SDL_RendererFlip mFlip;
};

class Enemy : public Character {
public:
    Enemy(SDL_Renderer* render, LTexture* texture, SDL_Rect* clips, int numClips, int x, int y);
    void render() override;
    void render(int camX, int camY) override;
};

#endif // CHARACTER_H