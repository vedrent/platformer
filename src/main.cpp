#include <iostream>
#include <SDL2/SDL.h>
#include <string>
//#include "windows.h"
using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int CHARACTER_WIDTH = 50;
const int CHARACTER_HEIGHT = 50;
const int GRAVITATION = 2;
const int RIGHTWALLCOLLISION = 1;
const int LEFTWALLCOLLISION = 2;
const int TOPWALLCOLLISION = 3;


SDL_Window *window = nullptr;
SDL_Renderer *render = nullptr;
//SDL_Surface *walter = nullptr;
SDL_Texture *background = nullptr;
SDL_Texture *level = nullptr;
SDL_Surface *level_surf = nullptr;
SDL_Texture *among = nullptr;
SDL_Surface *test_surf = nullptr;
SDL_Texture *test_text = nullptr;

int curr_x = 0;
int curr_y = SCREEN_HEIGHT/2 - CHARACTER_HEIGHT/2;
int vel_x = 0;
int vel_y = 0;


bool init(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Testing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

SDL_Rect MakeRect(int x, int y, int w, int h){
    SDL_Rect temp_rect;
    temp_rect.x = x;
    temp_rect.y = y;
    temp_rect.w = w;
    temp_rect.h = h;
    return temp_rect;
}

SDL_Texture* LoadTexture(string file, string color_key){
    SDL_Surface *loadedImage = nullptr;
    SDL_Texture *texture = nullptr;
    loadedImage = SDL_LoadBMP(file.c_str());
    if (loadedImage != nullptr){
        if (color_key == "white"){
            SDL_SetColorKey(loadedImage, SDL_TRUE, SDL_MapRGBA(loadedImage->format, 0xFF, 0xFF, 0xFF, 0xFF));
        }
        texture = SDL_CreateTextureFromSurface(render, loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    else{
        cout << SDL_GetError() << endl;
    }
    return texture;
}

SDL_Surface* LoadSurface(string file){
    SDL_Surface *loadedImage = nullptr;
    loadedImage = SDL_LoadBMP(file.c_str());
    loadedImage = SDL_ConvertSurfaceFormat(loadedImage, SDL_GetWindowPixelFormat(window), 0);
    return loadedImage;
}

Uint32 GetPixel(Uint32 x, Uint32 y, SDL_Surface* surf){
    Uint32* pixels = static_cast<Uint32*>(surf->pixels);
    return pixels[y*(surf->pitch)/4 + x];
}

bool OnFloor(){
    for (int size = 0; size < CHARACTER_WIDTH; size++){
        if (GetPixel(SCREEN_WIDTH/2 - CHARACTER_WIDTH/2 - curr_x + size, curr_y + CHARACTER_HEIGHT, level_surf) != GetPixel(0, 0, level_surf)){
            return true;
        }
    }
    return false;
}
bool InTexture(){
    for (int size = 6; size < CHARACTER_WIDTH-6; size++){
        if (GetPixel(SCREEN_WIDTH/2 - CHARACTER_WIDTH/2 - curr_x + size, curr_y + CHARACTER_HEIGHT - 1, level_surf) != GetPixel(0, 0, level_surf)){
            return true;
        }
    }
    return false;
}

int CollisionWithWalls(){
    for (int height = 0; height < CHARACTER_HEIGHT; height++){
        if (GetPixel(SCREEN_WIDTH/2 + CHARACTER_WIDTH/2 - curr_x - 1, curr_y + height, level_surf) != GetPixel(0, 0, level_surf)){
            return RIGHTWALLCOLLISION;
        }
        if (GetPixel(SCREEN_WIDTH/2 - CHARACTER_WIDTH/2 - curr_x + 1, curr_y + height, level_surf) != GetPixel(0, 0, level_surf)){
            return LEFTWALLCOLLISION;
        }
    }
    for (int width = 0; width < CHARACTER_WIDTH; width++){
        if (GetPixel(SCREEN_WIDTH/2 - CHARACTER_WIDTH/2 - curr_x + width, curr_y + 1, level_surf) != GetPixel(0, 0, level_surf)){
            return TOPWALLCOLLISION;
        }
    }
    return 0;
}

void ApplySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend){
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    SDL_QueryTexture(tex, nullptr, nullptr, &pos.w, &pos.h);
    SDL_RenderCopy(rend, tex, nullptr, &pos);
}

void Quit(){
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(level);
    SDL_DestroyTexture(among);
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

    test_surf = LoadSurface("../res/2x5px.bmp");
    test_text = LoadTexture("../res/2x5px.bmp", "none");

//    walter = LoadSurface("../res/walter.bmp");
    background = LoadTexture("../res/Background.bmp", "none");
    level_surf = LoadSurface("../res/level.bmp");
    level = LoadTexture("../res/level.bmp", "white");
    among = LoadTexture("../res/among.bmp", "white");

    bool quit = false;
    SDL_Rect back_rect;
    SDL_Rect level_rect;
    SDL_Rect among_rect;
    while (!quit){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch (event.key.keysym.sym) {
                case SDLK_DOWN:
                    curr_y+=5;
                    break;
                case SDLK_UP:
                    curr_y-=5;
                    break;
                case SDLK_RIGHT:
                    curr_x-=10;
                    break;
                case SDLK_LEFT:
                    curr_x+=10;
                    break;
                case SDLK_SPACE:
                    vel_y-=20;
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                default:
                    break;
            }
            if (event.type == SDL_QUIT){
                quit = true;
            }
        }

        if (vel_y != 0){
            curr_y = curr_y + vel_y;
        }

        if (!OnFloor()){
            vel_y += GRAVITATION;
        } else{
            vel_y = 0;
        }
        bool quit_flag = true;
        while (quit_flag){
            switch (CollisionWithWalls()) {
                case RIGHTWALLCOLLISION:
                    if (!InTexture()) {
                        curr_x++;
                    } else{
                        quit_flag = false;
                        break;
                    }
                case LEFTWALLCOLLISION:
                    if (!InTexture()) {
                        curr_x--;
                    } else{
                        quit_flag = false;
                        break;
                    }
                case TOPWALLCOLLISION:
                    curr_y++;
                case 0:
                    quit_flag = false;
                    break;
            }
        }
        while (InTexture()){
            curr_y--;
        }

        if (curr_y < 0){
            curr_y = 0;
        }
        if (curr_x > 0){
            curr_x = 0;
        }
        if (curr_x < -SCREEN_WIDTH*2){
            curr_x = 1 - SCREEN_WIDTH*2;
        }
        
        back_rect = MakeRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        level_rect = MakeRect(curr_x, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT);
        among_rect = MakeRect(SCREEN_WIDTH/2 - CHARACTER_WIDTH/2, curr_y, CHARACTER_WIDTH, CHARACTER_HEIGHT);
        SDL_RenderClear(render);
        SDL_RenderCopy(render, background, nullptr, &back_rect);
        SDL_RenderCopy(render, level, nullptr, &level_rect);
        SDL_RenderCopy(render, among, nullptr, &among_rect);
        SDL_RenderPresent(render);

        SDL_Delay(10);
    }

    Quit();
    return 0;
}
