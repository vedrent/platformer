#pragma once

#include "SDL2/SDL.h"
#include "string"
#include "Texture.h"

using namespace std;

class Character {
public:
    Character();

    Character(Texture texture, int x, int y, int w, int h, int vl_y, int accel);

    Uint32 GetPixel(Uint32 x, Uint32 y, SDL_Surface *surf);

    bool OnFloor(Texture level);

    bool InTexture(Texture level);

    int CollisionWithWalls(Texture level);

    void Move(Texture level, Uint8* state);

    void Render(SDL_Renderer *render);

    void SetX(int new_x);

    void SetY(int new_y);

    void SetVelocity(int new_vel);

    void SetAcceleration(int new_acc);

    Texture GetTexture();

    int GetX();

    int GetY();

    int GetVelocity();

    int GetAcceleration();

private:
    Texture cTexture;
    int sprite_curr;
    SDL_Rect* sprite_clips;
    int width;
    int height;
    int curr_x;
    int curr_y;
    int vel_y;
    int acceleration;
    bool is_on_jump;
    int t;
    int start_y;
};