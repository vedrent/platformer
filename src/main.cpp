#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include "Texture.h"
#include "Character.h"
#include "Enemy.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int CHARACTER_WIDTH = 60;
const int CHARACTER_HEIGHT = 100;
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
Texture karlson = Texture();
Texture mushroom = Texture();
Texture youdied = Texture();
Texture youwin = Texture();
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
    karlson.Free();
    mushroom.Free();
    youdied.Free();
    youwin.Free();
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

    const Uint8* state = SDL_GetKeyboardState(nullptr);

    back.LoadFromFile("../res/Background.bmp", "none", render, window);
    level.LoadFromFile("../res/level.bmp", "white", render, window);
    among.LoadFromFile("../res/among.bmp", "white", render, window);
    karlson.LoadFromFile("../res/karlson.bmp", "white", render, window);
    mushroom.LoadFromFile("../res/mushroom.bmp", "white", render, window);
    youdied.LoadFromFile("../res/youdied.bmp", "white", render, window);
    youwin.LoadFromFile("../res/youwin.bmp", "white", render, window);
    character = Character(karlson, 0, SCREEN_HEIGHT / 2 - CHARACTER_HEIGHT / 2, CHARACTER_WIDTH, CHARACTER_HEIGHT, 0, 0);
    enemy = Enemy(mushroom, 1300, 600 - ENEMY_HEIGHT, ENEMY_WIDTH, ENEMY_HEIGHT);
    // enemy cords 1300 600
    bool win = false;
    bool death = false;
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
//                case SDL_KEYDOWN:
//                    KEYS[event.key.keysym.sym] = true;
//                    break;
//                case SDL_KEYUP:
//                    KEYS[event.key.keysym.sym] = false;
//                    break;
                default:
                    break;
            }
            if (event.key.keysym.sym == SDLK_ESCAPE){
                quit = true;
            }
//                switch (event.key.keysym.sym) {
//                    case SDLK_RIGHT:
//                        character.SetX(character.GetX() - 10);
//                        enemy.SetX(enemy.GetX() - 10);
//                        break;
//                    case SDLK_LEFT:
//                        character.SetX(character.GetX() + 10);
//                        enemy.SetX(enemy.GetX() + 10);
//                        break;
//                    case SDLK_SPACE:
//                        if (event.key.repeat == 0) {
//                            character.SetAcceleration(character.GetAcceleration() - JUMP_POWER);
//                        }
//                        break;
//                    case SDLK_ESCAPE:
//                        quit = true;
//                        break;
//                    default:
//                        break;
//
//            }
//            if (event.type == SDL_QUIT) {
//                quit = true;
//            }
        }
        state = SDL_GetKeyboardState(nullptr);

        // CHARACTER
        character.Move(level, const_cast<Uint8 *>(state));
        if (character.GetX() < -1850 and character.GetY() < 180){
            quit = true;
            win = true;
        }

        // ENEMY
        enemy.Move(level, const_cast<Uint8 *>(state));
        if (enemy.CollisionWithCharacter(character)) {
            quit = true;
            death = true;
        }

        // RENDER
        SDL_RenderClear(render);
        back.Render(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, render);
        level.Render(character.GetX(), 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT, render);
        character.Render(render);
        enemy.Render(render);
        if (death) youdied.Render(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, render);
        if (win) youwin.Render(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, render);
        SDL_RenderPresent(render);

        SDL_Delay(20);
    }

    // DEATH CASE
    while (death) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        death = false;
                        break;
                    default:
                        break;
                }
            }
            if (event.type == SDL_QUIT) {
                death = false;
            }
        }
    }
    // WIN CASE
    while (win) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        win = false;
                        break;
                    default:
                        break;
                }
            }
            if (event.type == SDL_QUIT) {
                win = false;
            }
        }
    }

    Quit();
    return 0;
}
