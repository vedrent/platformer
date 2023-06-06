#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include "Texture.h"
#include "Character.h"
#include "Enemy.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int CHARACTER_WIDTH = 50;
const int CHARACTER_HEIGHT = 50;
const int ENEMY_HEIGHT = 50;
const int ENEMY_WIDTH = 50;
const int GRAVITATION = 1;
const int JUMP_POWER = 5;
const int RIGHTWALLCOLLISION = 1;
const int LEFTWALLCOLLISION = 2;
const int TOPWALLCOLLISION = 3;


SDL_Window *window = nullptr;
SDL_Renderer *render = nullptr;
Texture back = Texture();
Texture level = Texture();
Texture among = Texture();
Texture mushroom = Texture();
Character character = Character();
Enemy enemy = Enemy();


bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        return false;
    }
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (render == nullptr) {
        cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        return false;
    }

    return true;
}

void Quit() {
    back.Free();
    level.Free();
    among.Free();
    mushroom.Free();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
    SDL_Quit();
}

int main(int argv, char **args) {
    if (!init()) {
        system("pause");
        Quit();
        return 1;
    }

    back.LoadFromFile("../res/Background.bmp", "none", render, window);
    level.LoadFromFile("../res/level.bmp", "white", render, window);
    among.LoadFromFile("../res/among.bmp", "white", render, window);
    mushroom.LoadFromFile("../res/mushroom.bmp", "transparent", render, window);
    character = Character(among, 0, SCREEN_HEIGHT / 2 - CHARACTER_HEIGHT / 2, CHARACTER_WIDTH, CHARACTER_HEIGHT, 0, 0);
    enemy = Enemy(mushroom, 1300, 600 - ENEMY_HEIGHT, ENEMY_WIDTH, ENEMY_HEIGHT);
    // enemy cords 1300 600

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT:
                        character.SetX(character.GetX() - 10);
                        enemy.SetX(enemy.GetX() - 10);
                        break;
                    case SDLK_LEFT:
                        character.SetX(character.GetX() + 10);
                        enemy.SetX(enemy.GetX() + 10);
                        break;
                    case SDLK_SPACE:
                        if (event.key.repeat == 0) {
                            character.SetAcceleration(character.GetAcceleration() - JUMP_POWER);
                        }
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        break;
                }
            }
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }


        // CHARACTER
        if (character.GetY() != 0) {
            character.SetY(character.GetY() + character.GetVelocity());
        }

        if (!character.OnFloor(level)) {
            character.SetAcceleration(character.GetAcceleration() + GRAVITATION);
        } else {
//            character.SetVelocity(0);
        }
        character.SetVelocity(character.GetVelocity() + character.GetAcceleration());
        bool quit_flag = true;
        while (quit_flag) {
            switch (character.CollisionWithWalls(level)) {
                case RIGHTWALLCOLLISION:
                    if (!character.InTexture(level)) {
                        character.SetX(character.GetX() + 1);
                    } else {
                        quit_flag = false;
                        break;
                    }
                case LEFTWALLCOLLISION:
                    if (!character.InTexture(level)) {
                        character.SetX(character.GetX() - 1);
                    } else {
                        quit_flag = false;
                        break;
                    }
                case TOPWALLCOLLISION:
                    character.SetY(character.GetY() + 1);
                case 0:
                    quit_flag = false;
                    break;
            }
        }
        while (character.InTexture(level)) {
            character.SetY(character.GetY() - 1);
            character.SetVelocity(0);
            character.SetAcceleration(0);
        }

        if (character.GetY() < 0) {
            character.SetY(0);
        }
        if (character.GetX() > 0) {
            character.SetX(0);
        }
        if (character.GetX() < -SCREEN_WIDTH * 2) {
            character.SetX(1 - SCREEN_WIDTH * 2);
        }

        // ENEMY
        enemy.Move(level);


        // RENDER
        SDL_RenderClear(render);
        back.Render(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, render);
        level.Render(character.GetX(), 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT, render);
        character.Render(render);
        enemy.Render(render);
        SDL_RenderPresent(render);

        SDL_Delay(20);
    }

    Quit();
    return 0;
}
