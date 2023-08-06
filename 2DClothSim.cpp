// 2DClothSim.cpp : Defines the entry point for the application.
//

#include "2DClothSim.h"

using namespace std;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    ClothSim::window = SDL_CreateWindow(
        "2DClothSim",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0
    );

    ClothSim::renderer = SDL_CreateRenderer(ClothSim::window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(ClothSim::renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ClothSim::renderer);

    ClothSim::Cloth* cloth = new ClothSim::Cloth();
    SDL_SetRenderDrawColor(ClothSim::renderer, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPointsF(ClothSim::renderer, cloth->getVerts()->data(), cloth->getPixelsX() * cloth->getPixelsY());

    SDL_RenderPresent(ClothSim::renderer);
    ClothSim::waitForExit();

    SDL_DestroyWindow(ClothSim::window);
    SDL_Quit();

    return 0;
}

void ClothSim::waitForExit() {
    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e))
            quit = (e.type == SDL_QUIT);
    }
}

void ClothSim::Cloth::initVerts() {
    verts = new std::vector<SDL_FPoint>;

    for (int64_t i = 0; i < pixels_y; i++) {
        for (int64_t j = 0; j < pixels_x; j++) {
            verts->push_back(SDL_FPoint());
            (*verts)[(i * pixels_x) + j].x = (float)(10 + 2 * j);
            (*verts)[(i * pixels_x) + j].y = (float)(10 + 2 * i);
        }
    }

    init = true;
}
