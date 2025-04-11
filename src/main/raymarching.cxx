#include <iostream>
#include <string>
#include <windows.h>
#include <math.h>
#include <vector>
#include <map>
#include <utility>
#include <float.h>
#include <algorithm>

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

    // This code is cursed but I just want it to work for now :)
    Shape* combinedShape1 = new CombinedShape(new Cube(Vec3(24, 0, 0), 16), new Cube(Vec3(20, 0, 0), 12), true);
    Shape* combinedShape2 = new CombinedShape(new Sphere(Vec3(20, 0, 0), 2.5), new Cube(Vec3(20, 0, 0), 4), false);
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
    double dot = (3 * std::max(dot1, 0.0) + std::max(dot2, 0.0)) / 4;

    delete combinedShape1;
    delete combinedShape2;

    return {endDistance, minDistanceTotal, iter, dot};
};

RGB getColor(RayMarchInfo info)
{

    if (info.distance == -1)
    {
        int r = 0;
        int g = 100 / std::max(1.0, info.minDistance);
        int b = 200 / std::max(1.0, info.minDistance);
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
    color.r = (rgbInt & 0xFF000000) >> 24;
    color.g = (rgbInt & 0x00FF0000) >> 16;
    color.b = (rgbInt & 0x0000FF00) >> 8;
    return color;
}

Uint32 toInt(RGB color)
{
    return (color.r << 24) + (color.b << 16) + (color.b << 8);
}

int getPixelIndex(int i, int j, int W)
{
    return (j * W + i);
}

RGB getPixelColor(int i, int j, Vec3 startPosition, double startPhi, double startTheta, int W, int H)
{
    double phi_range = M_PI / 3;
    double theta_range = M_PI / 2;
    double phi = startPhi - phi_range / 2 + phi_range * j / H;
    double theta = startTheta - theta_range / 2 + theta_range * i / W;

    RayMarchInfo info = rayMarchDistance(startPosition, phi, theta);
    RGB color = getColor(info);
    return color;
}

void setRenderedPixels(Uint32 *pixels, const int W, const int H, Vec3 startPosition, double startPhi, double startTheta)
{   

    int pixelSkip = 10;
    bool* rendered = new bool[W*H];
    for (int i = 0; i < W*H; i++) {
        rendered[i] = false;
    }

    for (int i = 0; i < W - pixelSkip; i += pixelSkip)
    {
        for (int j = 0; j < H - pixelSkip; j += pixelSkip)
        {

            RGB colorUL;
            if (rendered[getPixelIndex(i, j, W)])
            {
                colorUL = toRGB(pixels[getPixelIndex(i, j, W)]);
            }
            else
            {
                colorUL = getPixelColor(i, j, startPosition, startPhi, startTheta, W, H);
            }
            RGB colorUR;
            if (rendered[getPixelIndex(i + pixelSkip, j, W)])
            {
                colorUR = toRGB(pixels[getPixelIndex(i + pixelSkip, j, W)]);
            }
            else
            {
                colorUR = getPixelColor(i + pixelSkip, j, startPosition, startPhi, startTheta, W, H);
            }
            RGB colorDR;
            if (rendered[getPixelIndex(i + pixelSkip, j + pixelSkip, W)])
            {
                colorDR = toRGB(pixels[getPixelIndex(i + pixelSkip, j + pixelSkip, W)]);
            }
            else
            {
                colorDR = getPixelColor(i + pixelSkip, j + pixelSkip, startPosition, startPhi, startTheta, W, H);
            }
            RGB colorDL;
            if (rendered[getPixelIndex(i, j + pixelSkip, W)])
            {
                colorDL = toRGB(pixels[getPixelIndex(i, j + pixelSkip, W)]);
            }
            else
            {
                colorDL = getPixelColor(i, j + pixelSkip, startPosition, startPhi, startTheta, W, H);
            }

            for (int x = 0; x <= pixelSkip; x++)
            {
                for (int y = 0; y <= pixelSkip; y++)
                {
                    double N1 = (pixelSkip - x) * (pixelSkip - y) / static_cast<double>(pixelSkip * pixelSkip);
                    double N2 = (x) * (pixelSkip - y) / static_cast<double>(pixelSkip * pixelSkip);
                    double N3 = (x) * (y) / static_cast<double>(pixelSkip * pixelSkip);
                    double N4 = (pixelSkip - x) * (y) / static_cast<double>(pixelSkip * pixelSkip);

                    RGB color;
                    color.r = (int)(N1 * colorUL.r + N2 * colorUR.r + N3 * colorDR.r + N4 * colorDL.r);
                    color.g = (int)(N1 * colorUL.g + N2 * colorUR.g + N3 * colorDR.g + N4 * colorDL.g);
                    color.b = (int)(N1 * colorUL.b + N2 * colorUR.b + N3 * colorDR.b + N4 * colorDL.b);

                    pixels[getPixelIndex(i + x, j + y, W)] = toInt(color);
                    rendered[getPixelIndex(i + x, j + y, W)] = true;
                }
            }
        }
    }
    delete[] rendered;
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
    int W = 1000;
    int H = 1000;

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