#include <iostream>
#include <string>

#include <windows.h>
#include "SDL.h"

#include "Sphere.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(3000);

    SDL_DestroyWindow(window);
    SDL_Quit();

    Sphere* sphere = new Sphere(1, 0, 0, 1);

    double dist = sphere->getMinimumDistance(0, 0, 0);

    std::cout << "Hej värld!" << dist << std::endl;
    std::string name;
    std::cin >> name;
    std::cout << name << std::endl;

    return 0;
}