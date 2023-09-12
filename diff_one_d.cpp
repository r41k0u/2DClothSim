#include "2DClothSim.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    ClothSim::window = SDL_CreateWindow(
        "diff_one_d",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0
    );

    ClothSim::renderer = SDL_CreateRenderer(ClothSim::window, -1, SDL_RENDERER_SOFTWARE);
    ClothSim::OneDSystem* springs = new ClothSim::OneDSystem();

    SDL_Event e;
    do {
        SDL_SetRenderDrawColor(ClothSim::renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(ClothSim::renderer);
        springs->update(0.02f);
        SDL_SetRenderDrawColor(ClothSim::renderer, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPointsF(ClothSim::renderer, springs->getVerts()->data(), (int32_t)(springs->getPixelsX() * springs->getPixelsY()));
        SDL_RenderPresent(ClothSim::renderer);
        SDL_PollEvent(&e);
    } while (e.type != SDL_QUIT);

    SDL_DestroyWindow(ClothSim::window);
    SDL_Quit();

    return 0;
}

void ClothSim::OneDSystem::updateAcc(const double time) {
    for (int64_t i = 0; i < pixels_y; i++) {
        for (int64_t j = 0; j < pixels_x; j++)
            (*accs)[(pixels_x * i) + j] = GRAVITY;
    }
    calcStruct(time);

    // Fixed the 2 corners of cloth
    (*accs)[0].x = (*accs)[0].y = (*accs)[pixels_x - 1].x = (*accs)[pixels_x - 1].y = .0f;
}
