#pragma once

#include "Texture.h"
#include "SDL2/SDL.h"
#include "string"

using namespace std;

Texture::Texture() {
    aSurface = nullptr;
    aTexture = nullptr;
    aWidth = 0;
    aHeight = 0;
}

bool Texture::LoadFromFile(string path, string color_key, SDL_Renderer *render, SDL_Window *window) {
    Free();
    SDL_Surface *loadedImage = SDL_LoadBMP(path.c_str());
    if (loadedImage != nullptr) {
        if (color_key == "white") {
            SDL_SetColorKey(loadedImage, SDL_TRUE, SDL_MapRGBA(loadedImage->format, 0xFF, 0xFF, 0xFF, 0xFF));
        }
        if (color_key == "transparent"){
            SDL_SetColorKey(loadedImage, SDL_TRUE, SDL_MapRGBA(loadedImage->format, 0xFF, 0xFF, 0xFF, 0x00));
        }
        aWidth = loadedImage->w;
        aHeight = loadedImage->h;
        aTexture = SDL_CreateTextureFromSurface(render, loadedImage);
        aSurface = SDL_ConvertSurfaceFormat(loadedImage, SDL_GetWindowPixelFormat(window), 0);
        SDL_FreeSurface(loadedImage);
    }
    return aTexture != nullptr;
}

void Texture::Free() {
    if (aTexture != nullptr) {
        SDL_DestroyTexture(aTexture);
        aSurface = nullptr;
        aWidth = 0;
        aHeight = 0;
    }
}

void Texture::Render(int x, int y, int width, int height, SDL_Renderer *render) {
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(render, aTexture, nullptr, &rect);
}

SDL_Surface *Texture::GetSurface() {
    return aSurface;
}

SDL_Texture *Texture::GetTexture() {
    return aTexture;
}

int Texture::GetWidth() {
    return aWidth;
}

int Texture::GetHeight() {
    return aHeight;
}
