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
    ClothSim::Cloth* cloth = new ClothSim::Cloth();
    SDL_Event e;
    do {
        SDL_SetRenderDrawColor(ClothSim::renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(ClothSim::renderer);
        ClothSim::update(cloth, 0.05f);
        SDL_SetRenderDrawColor(ClothSim::renderer, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPointsF(ClothSim::renderer, cloth->getVerts()->data(), cloth->getPixelsX() * cloth->getPixelsY());
        SDL_RenderPresent(ClothSim::renderer);
        SDL_PollEvent(&e);
    } while (e.type != SDL_QUIT);

    SDL_DestroyWindow(ClothSim::window);
    SDL_Quit();

    return 0;
}

void ClothSim::Cloth::initVerts() {
    verts = new std::vector<SDL_FPoint>;
    velos = new std::vector<struct velo>;

    for (int64_t i = 0; i < pixels_y; i++) {
        for (int64_t j = 0; j < pixels_x; j++) {
            verts->push_back(SDL_FPoint());
            velos->push_back(velo(.0f, 1.0f));
            (*verts)[(i * pixels_x) + j].x = (float)(10 + 2 * j);
            (*verts)[(i * pixels_x) + j].y = (float)(10 + 2 * i);
        }
    }

    init = true;
}

void ClothSim::update(Cloth* cloth, const float time) {
    cloth->updatePos(time);
}

void ClothSim::Cloth::updatePos(const float time) {
    for (int i = 0; i < pixels_x * pixels_y; i++) {
        (*verts)[i].x += (*velos)[i].x * time;
        (*verts)[i].y += (*velos)[i].y * time;
    }
}
