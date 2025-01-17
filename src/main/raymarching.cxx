#include <iostream>
#include <string>
#include <windows.h>
#include <math.h>
#include <vector>
#include <map>
#include <float.h>

#include "SDL.h"
#include "Shape.h"
#include "Sphere.h"
#include "Vec3.h"

struct RayMarchInfo
{
    double distance;
    int iterations;
    double dot;
};

struct RGB
{
    int r;
    int g;
    int b;
};

double d(Vec3 v1, Vec3 v2)
{
    return (v1 - v2).getLength();
};

RayMarchInfo calculateDistance(Vec3 startx, double phi, double theta)
{   
    Vec3 x = startx;
    Vec3 dx = Vec3::getUnitVector(phi, theta);

    std::vector<Shape*> shapes = {&Sphere(Vec3(10, 0, 0), 3), &Sphere(Vec3(20, 10, 0), 3)};

    double minDistance = DBL_MAX;
    double maxDistance = DBL_MIN;
    double prevMinDistance = DBL_MAX;
    double prevMaxDistance = DBL_MIN;
    Shape* minShape = nullptr; 

    int iter = 0;
    double tol = 0.01;
    while (minDistance >= tol)
    {

        prevMinDistance = minDistance;
        prevMaxDistance = maxDistance;

        minDistance = DBL_MAX;
        maxDistance = DBL_MIN;

        for (Shape* shape : shapes)
        {
            double distance = shape->getMinimumDistance(x);
            if (distance < minDistance)
            {
                minDistance = distance;
                minShape = shape;
            }
            if (distance > maxDistance)
            {
                maxDistance = distance;
            }
        }

        // If moving away from all objects, terminate
        // Might be flawed
        if (minDistance > prevMinDistance && maxDistance > prevMaxDistance)
        {
            // -1 means infinite
            return {-1, iter, 0};
        }

        x = x + dx * minDistance;

        iter++;
    }

    double endDistance = d(x, startx);

    double dot = minShape->getNormal(x)*(startx - x)/(startx - x).getLength();

    return {endDistance, iter, dot};
};

RGB getColor(RayMarchInfo info)
{

    if (info.distance == -1)
    {
        return {0, 0, 0};
    }
    else
    {   
        double f = info.dot;
        return {(int) (55 + 200*f), (int) (55 + 200*f), (int) (55 + 200*f)};
    }
};

RGB toRGB(Uint32 rgbInt)
{
    RGB color;
    color.r = (rgbInt & 0xFF0000) >> 24;
    color.g = (rgbInt & 0x00FF00) >> 16;
    color.b = (rgbInt & 0x0000FF) >> 8;
    return color;
}

Uint32 toInt(RGB color)
{
    return (color.r << 24) + (color.b << 16) + (color.b << 8);
}

void setRenderedPixels(Uint32* pixels, const int W, const int H, Vec3 startPosition, double startPhi, double startTheta)
{
    for (int i = 0; i < W; i++)
    {
        for (int j = 0; j < H; j++)
        {
            double phi = startPhi - M_PI / 3 + 2 * M_PI / 3 * j / H;
            double theta = startTheta - M_PI / 3 + 2 * M_PI / 3 * i / W;

            RayMarchInfo info = calculateDistance(startPosition, phi, theta);
            RGB color = getColor(info);
            pixels[(j * W + i)] = toInt(color);
        }
    }
};

void handleInput(std::map<int, bool> keyboard, Vec3 &pos, double &phi, double &theta)
{

    if (keyboard[SDL_KeyCode::SDLK_w])
    {
        pos = pos + Vec3::getUnitVector(M_PI/2, theta);
    }
    if (keyboard[SDL_KeyCode::SDLK_s])
    {
        pos = pos + Vec3::getUnitVector(M_PI/2, theta + M_PI);
    }
    if (keyboard[SDL_KeyCode::SDLK_a])
    {
        pos = pos + Vec3::getUnitVector(M_PI/2, theta - M_PI/2);
    }
    if (keyboard[SDL_KeyCode::SDLK_d])
    {
        pos = pos + Vec3::getUnitVector(M_PI/2, theta + M_PI/2);
    }
    if (keyboard[SDL_KeyCode::SDLK_SPACE])
    {
        pos = pos + Vec3::getUnitVector(0, 0);
    }
    if (keyboard[SDL_KeyCode::SDLK_LSHIFT])
    {
        pos = pos + Vec3::getUnitVector(M_PI, 0);
    }
    if (keyboard[SDL_KeyCode::SDLK_DOWN])
    {
        phi = phi + 0.1;
    }
    if (keyboard[SDL_KeyCode::SDLK_UP])
    {
        phi = phi - 0.1;
    }
    if (keyboard[SDL_KeyCode::SDLK_RIGHT])
    {
        theta = theta + 0.1;
    }
    if (keyboard[SDL_KeyCode::SDLK_LEFT])
    {
        theta = theta - 0.1;
    }
}

int main(int argc, char *argv[])
{
    int W = 300;
    int H = 300;

    Vec3 pos = Vec3(0, 0, 0);
    double phi = M_PI / 2;
    double theta = 0;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        W,
        H,
        0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, W, H);
    std::map<int, bool> keyboard;

    while (true)
    {
        void* pixels = new Uint32[W*H];
        int pitch;
        SDL_LockTexture(texture, NULL, &pixels, &pitch);
        setRenderedPixels((Uint32*) pixels, W, H, pos, phi, theta);
        SDL_UnlockTexture(texture);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                keyboard[event.key.keysym.sym] = true;
                break;
            case SDL_KEYUP:
                keyboard[event.key.keysym.sym] = false;
                break;
            }
        }

        handleInput(keyboard, pos, phi, theta);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
};