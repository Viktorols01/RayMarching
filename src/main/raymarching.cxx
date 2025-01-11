#include <iostream>
#include <string>

#include <windows.h>
#include "SDL.h"
#include "renderspace.cxx"

int main(int argc, char* argv[]) {
    int W = 640;
    int H = 480;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        W,
        H,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    
    // Fill background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    
    renderspace(renderer, W, H);
    
    SDL_RenderPresent(renderer);

    SDL_Delay(3000);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
};