#pragma once

#include "SDL2/SDL.h"
#include "string"
#include "Texture.h"

using namespace std;

class Enemy {
public:
    Enemy();

    Enemy(Texture texture, int x, int y, int w, int h);

    Uint32 GetPixel(Uint32 x, Uint32 y, SDL_Surface *surf);

    bool LeftCollision(Texture level);

    bool RightCollision(Texture level);

    void Move(Texture level);

    void Render(SDL_Renderer *render);

    void SetX(int new_x);

    void SetY(int new_y);

    Texture GetTexture();

    int GetX();

    int GetY();

private:
    Texture cTexture;
    int width;
    int height;
    int curr_x;
    int curr_y;
    int level_x;
    int level_y;
    bool direction;
};
