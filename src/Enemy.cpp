#pragma once

#include "Enemy.h"
#include "SDL2/SDL.h"

using namespace std;

Enemy::Enemy() {
    cTexture = Texture();
    width = 0;
    height = 0;
    curr_x = 0;
    curr_y = 0;
    level_x = 0;
    level_y = 0;
    direction = true;
}

Enemy::Enemy(Texture texture, int x, int y, int w, int h) {
    cTexture = texture;
    width = w;
    height = h;
    curr_x = x;
    curr_y = y;
    level_x = x;
    level_y = y;
    direction = true;
}

Uint32 Enemy::GetPixel(Uint32 x, Uint32 y, SDL_Surface *surf) {
    Uint32 *pixels = static_cast<Uint32 *>(surf->pixels);
    return pixels[y * (surf->pitch) / 4 + x];
}

bool Enemy::LeftCollision(Texture level) {
    if (GetPixel(level_x - 1, level_y, level.GetSurface()) != GetPixel(0, 0, level.GetSurface())) {
        return true;
    }
    return false;
}

bool Enemy::RightCollision(Texture level) {
    if (GetPixel(level_x + width, level_y, level.GetSurface()) != GetPixel(0, 0, level.GetSurface())) {
        return true;
    }
    return false;
}

void Enemy::Move(Texture level) {
    if (direction) {
        if (!this->RightCollision(level)) {
            level_x++;
            curr_x++;
        } else {
            direction = false;
        }
    } else {
        if (!this->LeftCollision(level)) {
            level_x--;
            curr_x--;
        } else {
            direction = true;
        }
    }
}

void Enemy::Render(SDL_Renderer *render) {
    SDL_Rect rect = {curr_x, curr_y, width, height};
    SDL_RenderCopy(render, cTexture.GetTexture(), nullptr, &rect);
}

void Enemy::SetX(int new_x) {
    curr_x = new_x;
}

void Enemy::SetY(int new_y) {
    curr_y = new_y;
}

Texture Enemy::GetTexture() {
    return cTexture;
}

int Enemy::GetX() {
    return curr_x;
}

int Enemy::GetY() {
    return curr_y;
}