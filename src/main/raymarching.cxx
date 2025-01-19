#include <iostream>
#include <string>
#include <windows.h>
#include <math.h>
#include <vector>
#include <map>
#include <utility>
#include <float.h>

#include "SDL.h"
#include "Shape.h"
#include "Sphere.h"
#include "Cube.h"
#include "CombinedShape.h"
#include "InfiniSphere.h"
#include "Vec3.h"

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

struct RayMarchInfo
{
    double distance;
    double minDistance;
    int iterations;
    double dot;
};

RayMarchInfo rayMarchDistance(Vec3 startx, double phi, double theta)
{
    Vec3 x = startx;
    Vec3 dx = Vec3::getUnitVector(phi, theta);

    Shape* combinedShape1 = &CombinedShape(&Cube(Vec3(24, 0, 0), 16), &Cube(Vec3(20, 0, 0), 12), true);
    Shape* combinedShape2 = &CombinedShape(&Sphere(Vec3(20, 0, 0), 4), &Cube(Vec3(16, 0, 0), 2), true);
    std::vector<Shape *> shapes = {
        //&InfiniSphere(Vec3(5, 5, 5), 1, 30),
        combinedShape1,
        combinedShape2,
        };

    double minDistanceTotal = DBL_MAX;
    double minDistance = DBL_MAX;
    double maxDistance = DBL_MIN;
    Shape *minShape = nullptr;

    int iter = 0;
    double tol = 0.01;
    double render_radius = 100;
    while (minDistance >= tol)
    {

        minDistance = DBL_MAX;
        maxDistance = DBL_MIN;

        for (Shape *shape : shapes)
        {
            double distance = shape->getMinimumDistance(x);
            if (distance < minDistance)
            {
                minDistance = distance;
                minShape = shape;
            }
            if (distance < minDistanceTotal)
            {
                minDistanceTotal = distance;
            }
            if (distance > maxDistance)
            {
                maxDistance = distance;
            }
        }

        if (maxDistance > render_radius)
        {
            // -1 means infinite
            return {-1, minDistanceTotal, iter, 0};
        }

        x = x + dx * minDistance;

        iter++;
    }

    double endDistance = d(x, startx);

    double dot1 = minShape->getNormal(x) * (startx - x) / (startx - x).getLength();
    double dot2 = minShape->getNormal(x) * Vec3(0, 0, 1);
    double dot = (3*max(dot1, 0) + max(dot2, 0))/4;

    return {endDistance, minDistanceTotal, iter, dot};
};

RGB getColor(RayMarchInfo info)
{

    if (info.distance == -1)
    {
        int r = 0;
        int g = 100/max(1, info.minDistance);
        int b = 200/max(1, info.minDistance);
        return {r, g, b};
    }
    else
    {
        double f = info.dot;
        return {(int)(55 + 200 * f), (int)(55 + 200 * f), (int)(55 + 200 * f)};
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

void setRenderedPixels(Uint32 *pixels, const int W, const int H, Vec3 startPosition, double startPhi, double startTheta)
{
    for (int i = 0; i < W; i++)
    {
        for (int j = 0; j < H; j++)
        {
            double phi_range = M_PI / 3;
            double theta_range = M_PI / 2;
            double phi = startPhi - phi_range / 2 + phi_range * j / H;
            double theta = startTheta - theta_range / 2 + theta_range * i / W;

            RayMarchInfo info = rayMarchDistance(startPosition, phi, theta);
            RGB color = getColor(info);
            pixels[(j * W + i)] = toInt(color);
        }
    }
};

void handleInput(std::map<int, bool> keyboard, Vec3 &pos, double &phi, double &theta)
{

    if (keyboard[SDL_KeyCode::SDLK_w])
    {
        pos = pos + Vec3::getUnitVector(M_PI / 2, theta);
    }
    if (keyboard[SDL_KeyCode::SDLK_s])
    {
        pos = pos + Vec3::getUnitVector(M_PI / 2, theta + M_PI);
    }
    if (keyboard[SDL_KeyCode::SDLK_a])
    {
        pos = pos + Vec3::getUnitVector(M_PI / 2, theta - M_PI / 2);
    }
    if (keyboard[SDL_KeyCode::SDLK_d])
    {
        pos = pos + Vec3::getUnitVector(M_PI / 2, theta + M_PI / 2);
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
    int W = 200;
    int H = 200;

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
        void *pixels = new Uint32[W * H];
        int pitch;
        SDL_LockTexture(texture, NULL, &pixels, &pitch);
        setRenderedPixels((Uint32 *)pixels, W, H, pos, phi, theta);
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