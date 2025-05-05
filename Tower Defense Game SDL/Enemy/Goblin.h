#pragma once
#include "Enemy.h"

class Goblin : public Enemy {
public:
    static const int GOBLIN_HP = 200;
    static const int GOBLIN_SPEED = 50;

    Goblin(float x, float y, SDL_Renderer* renderer, int map[20][25], SDL_Texture* texture);
    ~Goblin();

    void display(SDL_Renderer* renderer) override;

private:
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;
    int frameWidth = 16;   // Width of a single frame 
    int frameHeight = 16;   // Height of a single frame
    int totalFrames = 4;   // Number of frames in the animation
    int currentFrame = 0;  // Current frame index
    Uint32 lastFrameTime = 0; // Last time the frame was updated
    Uint32 frameDelay = 100;
};
