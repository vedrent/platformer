#pragma once

#include "Character.h"
#include "SDL2/SDL.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int CHARACTER_WIDTH = 50;
const int CHARACTER_HEIGHT = 50;
const int RIGHTWALLCOLLISION = 1;
const int LEFTWALLCOLLISION = 2;
const int TOPWALLCOLLISION = 3;

Character::Character() {
    cTexture = Texture();
    width = 0;
    height = 0;
    curr_x = 0;
    curr_y = 0;
    vel_y = 0;
    acceleration = 0;
}

Character::Character(Texture texture, int x, int y, int w, int h, int vl_y, int accel) {
    cTexture = texture;
    width = w;
    height = h;
    curr_x = x;
    curr_y = y;
    vel_y = vl_y;
    acceleration = accel;
}

Uint32 Character::GetPixel(Uint32 x, Uint32 y, SDL_Surface *surf) {
    Uint32 *pixels = static_cast<Uint32 *>(surf->pixels);
    return pixels[y * (surf->pitch) / 4 + x];
}

bool Character::OnFloor(Texture level) {
    for (int size = 0; size < width; size++) {
        if (GetPixel(SCREEN_WIDTH / 2 - width / 2 - curr_x + size, curr_y + height, level.GetSurface()) !=
            GetPixel(0, 0, level.GetSurface())) {
            return true;
        }
    }
    return false;
}

bool Character::InTexture(Texture level) {
    for (int size = 6; size < width - 6; size++) {
        if (GetPixel(SCREEN_WIDTH / 2 - width / 2 - curr_x + size, curr_y + height - 1, level.GetSurface()) !=
            GetPixel(0, 0, level.GetSurface())) {
            return true;
        }
    }
    return false;
}

int Character::CollisionWithWalls(Texture level) {
    if (curr_y < 0) {
        return 0;
    }
    for (int h = 0; h < height; h++) {
        if (GetPixel(SCREEN_WIDTH / 2 + width / 2 - curr_x - 1, curr_y + h, level.GetSurface()) !=
            GetPixel(0, 0, level.GetSurface())) {
            return RIGHTWALLCOLLISION;
        }
        if (GetPixel(SCREEN_WIDTH / 2 - width / 2 - curr_x + 1, curr_y + h, level.GetSurface()) !=
            GetPixel(0, 0, level.GetSurface())) {
            return LEFTWALLCOLLISION;
        }
    }
    for (int size = 0; size < width; size++) {
        if (GetPixel(SCREEN_WIDTH / 2 - width / 2 - curr_x + size, curr_y + 1, level.GetSurface()) !=
            GetPixel(0, 0, level.GetSurface())) {
            return TOPWALLCOLLISION;
        }
    }
    return 0;
}

void Character::Render(SDL_Renderer *render) {
    SDL_Rect rect = {SCREEN_WIDTH / 2 - CHARACTER_WIDTH / 2, curr_y, width, height};
    SDL_RenderCopy(render, cTexture.GetTexture(), nullptr, &rect);
}

void Character::SetX(int new_x) {
    curr_x = new_x;
}

void Character::SetY(int new_y) {
    curr_y = new_y;
}

void Character::SetVelocity(int new_vel) {
    vel_y = new_vel;
}

void Character::SetAcceleration(int new_acc) {
    acceleration = new_acc;
}

Texture Character::GetTexture() {
    return cTexture;
}

int Character::GetX() {
    return curr_x;
}

int Character::GetY() {
    return curr_y;
}

int Character::GetVelocity() {
    return vel_y;
}

int Character::GetAcceleration() {
    return acceleration;
}