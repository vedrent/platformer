#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include "Texture.h"
#include "Character.h"
using namespace std;



const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int CHARACTER_WIDTH = 50;
const int CHARACTER_HEIGHT = 50;
const int GRAVITATION = 1;
const int JUMP_POWER = 5;
const int RIGHTWALLCOLLISION = 1;
const int LEFTWALLCOLLISION = 2;
const int TOPWALLCOLLISION = 3;


SDL_Window *window = nullptr;
SDL_Renderer *render = nullptr;
//SDL_Surface *walter = nullptr;
//SDL_Texture *background = nullptr;
//SDL_Texture *level = nullptr;
//SDL_Surface *level_surf = nullptr;
//SDL_Texture *among = nullptr;
//SDL_Surface *test_surf = nullptr;
//SDL_Texture *test_text = nullptr;

Texture back = Texture();
Texture level = Texture();
Texture among = Texture();
Character character = Character();

int curr_x = 0;
int curr_y = SCREEN_HEIGHT/2 - CHARACTER_HEIGHT/2;
int vel_x = 0;
int vel_y = 0;
int acceleration = 0;



bool init(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        return false;
    }
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (render == nullptr){
        cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        return false;
    }

    return true;
}

//SDL_Rect MakeRect(int x, int y, int w, int h){
//    SDL_Rect temp_rect;
//    temp_rect.x = x;
//    temp_rect.y = y;
//    temp_rect.w = w;
//    temp_rect.h = h;
//    return temp_rect;
//}

//SDL_Texture* LoadTexture(string file, string color_key){
//    SDL_Surface *loadedImage = nullptr;
//    SDL_Texture *texture = nullptr;
//    loadedImage = SDL_LoadBMP(file.c_str());
//    if (loadedImage != nullptr){
//        if (color_key == "white"){
//            SDL_SetColorKey(loadedImage, SDL_TRUE, SDL_MapRGBA(loadedImage->format, 0xFF, 0xFF, 0xFF, 0xFF));
//        }
//        texture = SDL_CreateTextureFromSurface(render, loadedImage);
//        SDL_FreeSurface(loadedImage);
//    }
//    else{
//        cout << SDL_GetError() << endl;
//    }
//    return texture;
//}

//SDL_Surface* LoadSurface(string file){
//    SDL_Surface *loadedImage = nullptr;
//    loadedImage = SDL_LoadBMP(file.c_str());
//    loadedImage = SDL_ConvertSurfaceFormat(loadedImage, SDL_GetWindowPixelFormat(window), 0);
//    return loadedImage;
//}

//Uint32 GetPixel(Uint32 x, Uint32 y, SDL_Surface* surf){
//    Uint32* pixels = static_cast<Uint32*>(surf->pixels);
//    return pixels[y*(surf->pitch)/4 + x];
//}

//bool OnFloor(){
//    for (int size = 0; size < CHARACTER_WIDTH; size++){
//        if (GetPixel(SCREEN_WIDTH/2 - CHARACTER_WIDTH/2 - curr_x + size, curr_y + CHARACTER_HEIGHT, level.GetSurface()) != GetPixel(0, 0, level.GetSurface())){
//            return true;
//        }
//    }
//    return false;
//}
//bool InTexture(){
//    for (int size = 6; size < CHARACTER_WIDTH-6; size++){
//        if (GetPixel(SCREEN_WIDTH/2 - CHARACTER_WIDTH/2 - curr_x + size, curr_y + CHARACTER_HEIGHT - 1, level.GetSurface()) != GetPixel(0, 0, level.GetSurface())){
//            return true;
//        }
//    }
//    return false;
//}
//int CollisionWithWalls(){
//    if (curr_y < 0){
//        return 0;
//    }
//    for (int height = 0; height < CHARACTER_HEIGHT; height++){
//        if (GetPixel(SCREEN_WIDTH/2 + CHARACTER_WIDTH/2 - curr_x - 1, curr_y + height, level.GetSurface()) != GetPixel(0, 0, level.GetSurface())){
//            return RIGHTWALLCOLLISION;
//        }
//        if (GetPixel(SCREEN_WIDTH/2 - CHARACTER_WIDTH/2 - curr_x + 1, curr_y + height, level.GetSurface()) != GetPixel(0, 0, level.GetSurface())){
//            return LEFTWALLCOLLISION;
//        }
//    }
//    for (int width = 0; width < CHARACTER_WIDTH; width++){
//        if (GetPixel(SCREEN_WIDTH/2 - CHARACTER_WIDTH/2 - curr_x + width, curr_y + 1, level.GetSurface()) != GetPixel(0, 0, level.GetSurface())){
//            return TOPWALLCOLLISION;
//        }
//    }
//    return 0;
//}

//void ApplySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend){
//    SDL_Rect pos;
//    pos.x = x;
//    pos.y = y;
//    SDL_QueryTexture(tex, nullptr, nullptr, &pos.w, &pos.h);
//    SDL_RenderCopy(rend, tex, nullptr, &pos);
//}

void Quit(){
    back.Free();
    level.Free();
    among.Free();
//    SDL_DestroyTexture(background);
//    SDL_DestroyTexture(level);
//    SDL_DestroyTexture(among);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
    SDL_Quit();
}

int main(int argv, char** args) {
    if (!init()){
        system("pause");
        Quit();
        return 1;
    }

//    test_surf = LoadSurface("../res/2x5px.bmp");
//    test_text = LoadTexture("../res/2x5px.bmp", "none");

//    walter = LoadSurface("../res/walter.bmp");
//    background = LoadTexture("../res/Background.bmp", "none");
//    level_surf = LoadSurface("../res/level.bmp");
//    level = LoadTexture("../res/level.bmp", "white");
//    among = LoadTexture("../res/among.bmp", "white");

    back.LoadFromFile("../res/Background.bmp", "none", render, window);
    level.LoadFromFile("../res/level.bmp", "white", render, window);
    among.LoadFromFile("../res/among.bmp", "white", render, window);
    character = Character(among, 0, SCREEN_HEIGHT/2 - CHARACTER_HEIGHT/2, CHARACTER_WIDTH, CHARACTER_HEIGHT, 0, 0);

//    SDL_Rect back_rect;
//    SDL_Rect level_rect;
//    SDL_Rect among_rect;
    bool quit = false;
    while (!quit){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    //                case SDLK_DOWN:
                    //                    curr_y+=5;
                    //                    break;
                    //                case SDLK_UP:
                    //                    curr_y-=5;
                    //                    break;
                    case SDLK_RIGHT:
                        character.SetX(character.GetX() - 10);
                        break;
                    case SDLK_LEFT:
                        character.SetX(character.GetX() + 10);
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
            if (event.type == SDL_QUIT){
                quit = true;
            }
        }

        if (character.GetY() != 0){
            character.SetY(character.GetY() + character.GetVelocity());
        }

        if (!character.OnFloor(level)){
            character.SetAcceleration(character.GetAcceleration() + GRAVITATION);
        } else{
//            character.SetVelocity(0);
        }
        character.SetVelocity(character.GetVelocity() + character.GetAcceleration());
        bool quit_flag = true;
        while (quit_flag){
            switch (character.CollisionWithWalls(level)) {
                case RIGHTWALLCOLLISION:
                    if (!character.InTexture(level)) {
                        character.SetX(character.GetX() + 1);
                    } else{
                        quit_flag = false;
                        break;
                    }
                case LEFTWALLCOLLISION:
                    if (!character.InTexture(level)) {
                        character.SetX(character.GetX() - 1);
                    } else{
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
        while (character.InTexture(level)){
            character.SetY(character.GetY() - 1);
            character.SetVelocity(0);
            character.SetAcceleration(0);
        }

        if (character.GetY() < 0){
            character.SetY(0);
        }
        if (character.GetX() > 0){
            character.SetX(0);
        }
        if (character.GetX() < -SCREEN_WIDTH*2){
            character.SetX(1 - SCREEN_WIDTH*2);
        }
        
//        back_rect = MakeRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
//        level_rect = MakeRect(curr_x, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT);
//        among_rect = MakeRect(SCREEN_WIDTH/2 - CHARACTER_WIDTH/2, curr_y, CHARACTER_WIDTH, CHARACTER_HEIGHT);
        SDL_RenderClear(render);
        back.Render(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, render);
        level.Render(character.GetX(), 0, SCREEN_WIDTH*2, SCREEN_HEIGHT, render);
        character.GetTexture().Render(SCREEN_WIDTH/2 - CHARACTER_WIDTH/2, character.GetY(), CHARACTER_WIDTH, CHARACTER_HEIGHT, render);
//        SDL_RenderCopy(render, background, nullptr, &back_rect);
//        SDL_RenderCopy(render, level, nullptr, &level_rect);
//        SDL_RenderCopy(render, among, nullptr, &among_rect);
        SDL_RenderPresent(render);

        SDL_Delay(20);
    }

    Quit();
    return 0;
}
