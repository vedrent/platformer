#pragma once

#include "Enemy.h"
#include "SDL2/SDL.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int CHARACTER_WIDTH = 60;
const int CHARACTER_HEIGHT = 100;
const int ENEMY_HEIGHT = 50;
const int ENEMY_WIDTH = 50;

Enemy::Enemy() {
    eTexture = Texture();
    sprite_curr = 0;
    sprite_clips = new SDL_Rect[8];
    for (int i = 0; i < 8; i++) {
        sprite_clips[i].x = i * ENEMY_WIDTH;
        sprite_clips[i].y = 0;
        sprite_clips[i].w = ENEMY_WIDTH;
        sprite_clips[i].h = ENEMY_HEIGHT;
    }
    width = 0;
    height = 0;
    curr_x = 0;
    curr_y = 0;
    level_x = 0;
    level_y = 0;
    direction = true;
}

Enemy::Enemy(Texture texture, int x, int y, int w, int h) {
    eTexture = texture;
    sprite_curr = 0;
    sprite_clips = new SDL_Rect[8];
    for (int i = 0; i < 8; i++) {
        sprite_clips[i].x = i * ENEMY_WIDTH;
        sprite_clips[i].y = 0;
        sprite_clips[i].w = ENEMY_WIDTH;
        sprite_clips[i].h = ENEMY_HEIGHT;
    }
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

bool Enemy::CollisionWithCharacter(Character character) {
    if (curr_x < SCREEN_WIDTH / 2 + CHARACTER_WIDTH / 2 and curr_x + width > SCREEN_WIDTH / 2 - CHARACTER_WIDTH / 2) {
        if (curr_y < character.GetY() + CHARACTER_HEIGHT and curr_y + height > character.GetY()) {
            return true;
        }
    }
    return false;
}

void Enemy::Move(Texture level, Uint8 *state, int x_per_frame) {
    curr_x += x_per_frame;

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
    SDL_RenderCopy(render, eTexture.GetTexture(), &sprite_clips[sprite_curr / 2], &rect);
    sprite_curr = (sprite_curr + 1) % 16;
}