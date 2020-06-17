// Introduction lab that covers:
// * C++
// * SDL
// * 2D graphics
// * Plotting pixels
// * Video memory
// * Color representation
// * Linear interpolation
// * glm::vec3 and std::vector

#include <SDL/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "SDLauxiliary.h"

using namespace std;
using glm::vec3;

// --------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Surface *screen;

vector<vec3> stars(1000);
int t;

// --------------------------------------------------------
// FUNCTION DECLARATIONS

void DrawColorInterpolation();

void Interpolate(float a, float b, vector<float> &result);

void Interpolate(const vec3 &a, const vec3 &b, vector<vec3> &result);

void InitializeStars();

void DrawStars();

void UpdateStars();

void Project(const vec3 &coord, int &outx, int &outy) {
    static constexpr int f = SCREEN_HEIGHT / 2;
    outx = f * (coord.x / coord.z) + SCREEN_WIDTH / 2;
    outy = f * (coord.y / coord.z) + SCREEN_HEIGHT / 2;
}

// --------------------------------------------------------
// FUNCTION DEFINITIONS

int main(int argc, char *argv[]) {
    screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT);
    InitializeStars();
    while (NoQuitMessageSDL()) {
        UpdateStars();
        DrawStars();
    }
    SDL_SaveBMP(screen, "screenshot.bmp");
    return 0;
}

void DrawColorInterpolation() {

    vec3 topLeft(1, 0, 0); // red
    vec3 topRight(0, 0, 1); // blue
    vec3 bottomLeft(1, 1, 0); // yellow
    vec3 bottomRight(0, 1, 0); // green

    vector<vec3> leftSide(SCREEN_HEIGHT);
    vector<vec3> rightSide(SCREEN_HEIGHT);
    Interpolate(topLeft, bottomLeft, leftSide);
    Interpolate(topRight, bottomRight, rightSide);

    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        vector<vec3> row(SCREEN_WIDTH);
        Interpolate(leftSide[y], rightSide[y], row);
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            vec3 color(0, 0, 1);
            PutPixelSDL(screen, x, y, row[x]);
        }
    }

    if (SDL_MUSTLOCK(screen))
        SDL_UnlockSurface(screen);

    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void Interpolate(float a, float b, vector<float> &result) {
    if (result.size() == 1) {
        result[0] = b;
        return;
    }
    for (size_t i = 0; i < result.size(); i++) {
        result[i] = a + ((b - a) / (result.size() - 1)) * i;
    }
}

void Interpolate(const vec3 &a, const vec3 &b, vector<vec3> &result) {
    if (result.size() == 1) {
        result[0] = b;
        return;
    }
    for (size_t i = 0; i < result.size(); i++) {
        const auto divisor = vec3(1.f, 1.f, 1.f).operator/=(result.size() - 1);
        result[i] = a + ((b - a) * divisor).operator*=(i);
    }
}

void InitializeStars() {
    for (auto &star : stars) {
        float rx = ((float(rand()) / float(RAND_MAX)) - 0.5f) * 2;
        float ry = ((float(rand()) / float(RAND_MAX)) - 0.5f) * 2;
        float rz = float(rand()) / float(RAND_MAX);
        star = vec3(rx, ry, rz == 0.f ? 0.5f : rz);
    }
    t = SDL_GetTicks();
}

void DrawStars() {
    if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
    SDL_FillRect(screen, 0, 0);
    for (size_t s = 0; s < stars.size(); ++s) {

        int px, py;
        Project(stars[s], px, py);
        vec3 color = 0.2f * vec3(1,1,1) / (stars[s].z*stars[s].z);
        PutPixelSDL(screen, px, py, color);

    }
    // Add code for projecting and drawing each star}
    SDL_UpdateRect(screen, 0, 0, 0, 0);
    if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
}

void UpdateStars() {

    static constexpr float v = 0.001f;

    int tt = t;
    int t2 = SDL_GetTicks();
    float dt = float(t2 - t);
    t = t2;

    assert(v*dt < 1.f);

    for (int s = 0; s < stars.size(); ++s) {
        stars[s].z -= v * dt;
        if (stars[s].z <= 0) {
            stars[s].z += 1;
        }
    }
}
