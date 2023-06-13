#pragma once

#include "Character.h"
#include "SDL2/SDL.h"
#include <cmath>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int CHARACTER_WIDTH = 60;
const int CHARACTER_HEIGHT = 100;
const int GRAVITATION = 1;
const int JUMP_POWER = 5;
const int RIGHTWALLCOLLISION = 1;
const int LEFTWALLCOLLISION = 2;
const int TOPWALLCOLLISION = 3;

const double timestep = 50;
const double timeup = 15; //seconds
const double jump_height = 200; //pixels
const double jump_power = (4 * jump_height / timeup) / timestep;
const double gravity = (2 * jump_power / timeup) / timestep;


Character::Character() {
    cTexture = Texture();
    sprite_curr = 0;
    sprite_clips = new SDL_Rect[8];
    for (int i = 0; i < 8; i++) {
        sprite_clips[i].x = i * CHARACTER_WIDTH;
        sprite_clips[i].y = 0;
        sprite_clips[i].w = CHARACTER_WIDTH;
        sprite_clips[i].h = CHARACTER_HEIGHT;
    }
    width = 0;
    height = 0;
    curr_x = 0;
    curr_y = 0;
    vel_y = 0;
    acceleration = 0;
    is_on_jump = false;
    t = 0;
    start_y = 0;
}

Character::Character(Texture texture, int x, int y, int w, int h, int vl_y, int accel) {
    cTexture = texture;
    sprite_curr = 0;
    sprite_clips = new SDL_Rect[8];
    for (int i = 0; i < 8; i++) {
        sprite_clips[i].x = i * CHARACTER_WIDTH;
        sprite_clips[i].y = 0;
        sprite_clips[i].w = CHARACTER_WIDTH;
        sprite_clips[i].h = CHARACTER_HEIGHT;
    }
    width = w;
    height = h;
    curr_x = x;
    curr_y = y;
    vel_y = vl_y;
    acceleration = accel;
    is_on_jump = false;
    t = 0;
    start_y = 0;
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
    for (int size = 11; size < width - 11; size++) {
        if (curr_y <= SCREEN_HEIGHT - height) {
            if (GetPixel(SCREEN_WIDTH / 2 - width / 2 - curr_x + size, curr_y + height - 1, level.GetSurface()) !=
                GetPixel(0, 0, level.GetSurface())) {
                return true;
            }
        } else {
            curr_y = SCREEN_HEIGHT - height;
        }
    }
    return false;
}

int Character::CollisionWithWalls(Texture level) {
    if (curr_y < 0) {
        return 0;
    }
    for (int h = 0; h < height - 1; h++) {
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

void Character::Move(Texture level, Uint8 *state) {
    if (state[SDL_SCANCODE_RIGHT]) {
        curr_x -= 10;
    }
    if (state[SDL_SCANCODE_LEFT]) {
        curr_x += 10;
    }
    if (state[SDL_SCANCODE_SPACE]) {
        if (OnFloor(level)) {
            is_on_jump = true;
            start_y = curr_y;
        }
    }

    if (is_on_jump) {
        if (!OnFloor(level) or t == 0) {
            t += 20;
            curr_y = start_y - jump_power * t + gravity * pow(t,2) / 2;
        } else{
            t = 0;
            is_on_jump = false;
        }
    }

    if (curr_y != 0) {
        curr_y += vel_y;
    }

    if (!this->OnFloor(level)) {
        acceleration += GRAVITATION;
    }
    vel_y += acceleration;
    bool quit_flag = true;
    while (quit_flag) {
        switch (this->CollisionWithWalls(level)) {
            case RIGHTWALLCOLLISION:
                if (!this->InTexture(level)) {
                    curr_x++;
                    break;
                } else {
                    quit_flag = false;
                    break;
                }
            case LEFTWALLCOLLISION:
                if (!this->InTexture(level)) {
                    curr_x--;
                    break;
                } else {
                    quit_flag = false;
                    break;
                }
            case TOPWALLCOLLISION:
                curr_y++;
                break;
            case 0:
                quit_flag = false;
                break;
        }
    }
    while (this->InTexture(level)) {
        curr_y--;
        vel_y = 0;
        acceleration = 0;
    }

    if (curr_y <= 0) {
        curr_y = 1;
        vel_y = 0;
        acceleration = 0;
    }
    if (curr_x > 0) {
        curr_x = 0;
    }
    if (curr_x < -SCREEN_WIDTH * 2) {
        curr_x = 1 - SCREEN_WIDTH * 2;
    }
}

void Character::Render(SDL_Renderer *render) {
    SDL_Rect rect = {SCREEN_WIDTH / 2 - CHARACTER_WIDTH / 2, curr_y, width, height};
    SDL_RenderCopy(render, cTexture.GetTexture(), &sprite_clips[sprite_curr / 2], &rect);
    sprite_curr = (sprite_curr + 1) % 16;
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