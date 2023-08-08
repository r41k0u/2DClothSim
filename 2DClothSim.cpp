// 2DClothSim.cpp : Defines the entry point for the application.
//

#include "2DClothSim.h"

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
        ClothSim::update(cloth, 0.002f);
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
    velos = new std::vector<struct vect2D>;
    accs = new std::vector<struct vect2D>;

    for (int64_t i = 0; i < pixels_y; i++) {
        for (int64_t j = 0; j < pixels_x; j++) {
            verts->push_back(SDL_FPoint());
            velos->push_back(vect2D(.0f, .0f));
            accs->push_back(vect2D(.0f, 9.8f));
            (*verts)[(i * pixels_x) + j].x = (float)(10 + 2 * j);
            (*verts)[(i * pixels_x) + j].y = (float)(10 + 2 * i);
        }
    }

    init = true;
}

void ClothSim::update(Cloth* cloth, const float time) {
    cloth->updateAcc(time);
    cloth->updateVelo(time);
    cloth->updatePos(time);
}

void ClothSim::Cloth::updateAcc(const float time) {
    calcStruct(time);
    calcShear(time);
    calcBend(time);

    // Fixed the 2 corners of cloth
    (*accs)[0].x = (*accs)[0].y = (*accs)[pixels_x - 1].x = (*accs)[pixels_x - 1].y = .0f;
}

void ClothSim::Cloth::updateVelo(const float time) {
    for (int i = 0; i < pixels_x * pixels_y; i++) 
        (*velos)[i] += (*accs)[i] * time;
}

void ClothSim::Cloth::updatePos(const float time) {
    for (int i = 0; i < pixels_x * pixels_y; i++) {
        (*verts)[i].x += (*velos)[i].x * time;
        (*verts)[i].y += (*velos)[i].y * time;
    }
}

void ClothSim::Cloth::calcStruct(const float time) {
    for (int64_t i = 0; i < pixels_y; i++) {
        for (int64_t j = 0; j < pixels_x; j++) {
            if (isValidVert(j + 1, i)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * i) + j + 1].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * i) + j + 1].y);
                struct vect2D acc = normalize(delPos) * (-springConst * (mag(delPos.x, delPos.y) - 2.0f));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j - 1, i)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * i) + j - 1].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * i) + j - 1].y);
                struct vect2D acc = normalize(delPos) * (-springConst * (mag(delPos.x, delPos.y) - 2.0f));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j, i + 1)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i + 1)) + j].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i + 1)) + j].y);
                struct vect2D acc = normalize(delPos) * (-springConst * (mag(delPos.x, delPos.y) - 2.0f));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j, i - 1)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i - 1)) + j].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i - 1)) + j].y);
                struct vect2D acc = normalize(delPos) * (-springConst * (mag(delPos.x, delPos.y) - 2.0f));
                (*accs)[(i * pixels_x) + j] += acc;
            }
        }
    }
}

void ClothSim::Cloth::calcShear(const float time) {
    for (int64_t i = 0; i < pixels_y; i++) {
        for (int64_t j = 0; j < pixels_x; j++) {
            if (isValidVert(j + 1, i + 1)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i + 1)) + j + 1].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i + 1)) + j + 1].y);
                struct vect2D acc = normalize(delPos) * (-springConst * (mag(delPos.x, delPos.y) - sqrt(8.0f)));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j - 1, i + 1)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i + 1)) + j - 1].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i + 1)) + j - 1].y);
                struct vect2D acc = normalize(delPos) * (-springConst * (mag(delPos.x, delPos.y) - sqrt(8.0f)));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j + 1, i - 1)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i - 1)) + j + 1].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i - 1)) + j + 1].y);
                struct vect2D acc = normalize(delPos) * (-springConst * (mag(delPos.x, delPos.y) - sqrt(8.0f)));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j - 1, i - 1)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i - 1)) + j - 1].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i - 1)) + j - 1].y);
                struct vect2D acc = normalize(delPos) * (-springConst * (mag(delPos.x, delPos.y) - sqrt(8.0f)));
                (*accs)[(i * pixels_x) + j] += acc;
            }
        }
    }
}

void ClothSim::Cloth::calcBend(const float time) {
    for (int64_t i = 0; i < pixels_y; i++) {
        for (int64_t j = 0; j < pixels_x; j++) {
            if (isValidVert(j + 2, i)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * i) + j + 2].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * i) + j + 2].y);
                struct vect2D acc = normalize(delPos) * (-springConst * (mag(delPos.x, delPos.y) - 4.0f));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j - 2, i)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * i) + j - 2].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * i) + j - 2].y);
                struct vect2D acc = normalize(delPos) * (-springConst * (mag(delPos.x, delPos.y) - 4.0f));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j, i + 2)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i + 2)) + j].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i + 2)) + j].y);
                struct vect2D acc = normalize(delPos) * (-springConst * (mag(delPos.x, delPos.y) - 4.0f));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j, i - 2)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i - 2)) + j].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i - 2)) + j].y);
                struct vect2D acc = normalize(delPos) * (-springConst * (mag(delPos.x, delPos.y) - 4.0f));
                (*accs)[(i * pixels_x) + j] += acc;
            }
        }
    }
}
