#pragma once

#include "SDL2/SDL.h"
#include "string"

using namespace std;

class Texture {
public:
    Texture();

    bool LoadFromFile(string path, string color_key, SDL_Renderer *render, SDL_Window *window);

    void Free();

    void Render(int x, int y, int width, int height, SDL_Renderer *render);

    SDL_Surface *GetSurface();

    SDL_Texture *GetTexture();

private:
    SDL_Texture *aTexture;
    SDL_Surface *aSurface;
    int aWidth;
    int aHeight;
};