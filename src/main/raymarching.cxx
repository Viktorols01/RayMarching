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

struct RayMarchInfo
{
    double distance;
    int iterations;
};

struct RGB
{
    int r;
    int g;
    int b;
};

struct Position
{
    double x;
    double y;
    double z;
};

double d(Position p1, Position p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.y - p2.y) * (p1.y - p2.y));
};

RayMarchInfo calculateDistance(double startX, double startY, double startZ, double phi, double theta)
{
    double x = startX;
    double y = startY;
    double z = startZ;

    double dx = sin(phi) * cos(theta);
    double dy = sin(phi) * sin(theta);
    double dz = cos(phi);

    std::vector<Shape *> shapes = {new Sphere(10, 0, 0, 3), new Sphere(20, 10, 0, 3)};

    double minDistance = DBL_MAX;
    double maxDistance = DBL_MIN;
    double prevMinDistance = DBL_MAX;
    double prevMaxDistance = DBL_MIN;
    int iter = 0;
    double tol = 0.01;
    while (minDistance >= tol)
    {

        prevMinDistance = minDistance;
        prevMaxDistance = maxDistance;

        minDistance = DBL_MAX;
        maxDistance = DBL_MIN;
        for (Shape *shape : shapes)
        {
            double distance = shape->getMinimumDistance(x, y, z);
            if (distance < minDistance)
            {
                minDistance = distance;
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
            return {-1, iter};
        }

        x = x + dx * minDistance;
        y = y + dy * minDistance;
        z = z + dz * minDistance;

        iter++;
    }

    double endDistance = d({x, y, z}, {startX, startY, startZ});
    return {endDistance, iter};
};

RGB getColor(RayMarchInfo info)
{

    if (info.distance == -1)
    {
        return {0, 0, 0};
    }
    else
    {
        return {255, 255, 255};
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

void setRenderedPixels(Uint32* pixels, const int W, const int H, Position startPosition, double startPhi, double startTheta)
{
    for (int i = 0; i < W; i++)
    {
        for (int j = 0; j < H; j++)
        {
            double phi = startPhi - M_PI / 3 + 2 * M_PI / 3 * j / H;
            double theta = startTheta - M_PI / 3 + 2 * M_PI / 3 * i / W;

            RayMarchInfo info = calculateDistance(startPosition.x, startPosition.y, startPosition.z, phi, theta);
            RGB color = getColor(info);
            pixels[(j * W + i)] = toInt(color);
        }
    }
};

void handleInput(std::map<int, bool> keyboard, Position &pos, double &phi, double &theta)
{

    if (keyboard[SDL_KeyCode::SDLK_w])
    {
        pos.x = pos.x + 1;
    }
    if (keyboard[SDL_KeyCode::SDLK_s])
    {
        pos.x = pos.x - 1;
    }
    if (keyboard[SDL_KeyCode::SDLK_a])
    {
        pos.y = pos.y - 1;
    }
    if (keyboard[SDL_KeyCode::SDLK_d])
    {
        pos.y = pos.y + 1;
    }
    if (keyboard[SDL_KeyCode::SDLK_SPACE])
    {
        pos.z = pos.z + 1;
    }
    if (keyboard[SDL_KeyCode::SDLK_LSHIFT])
    {
        pos.z = pos.z - 1;
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
    int W = 640;
    int H = 480;

    Position pos = {0, 0, 0};
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