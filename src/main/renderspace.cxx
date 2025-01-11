#include "SDL.h"

#include "Sphere.h"

void renderspace(SDL_Renderer* renderer, int W, int H) {
    // Fill pixels

    for (int i = 0; i < W; i++) {
        for (int j = 0; j < H; j++) {
            SDL_SetRenderDrawColor(renderer, 255*i/W, 255*j/H, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }

    Sphere* sphere = new Sphere(1, 0, 0, 1);

    double dist = sphere->getMinimumDistance(0, 0, 0);
}