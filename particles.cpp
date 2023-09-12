#include "2DClothSim.h"

void ClothSim::Particles::initVerts() {
    verts = new std::vector<SDL_FPoint>;
    velos = new std::vector<struct vect2D>;
    accs = new std::vector<struct vect2D>;

    for (int64_t i = 0; i < pixels_y; i++) {
        for (int64_t j = 0; j < pixels_x; j++) {
            verts->push_back(SDL_FPoint());
            velos->push_back(vect2D(.0f, .0f));
            accs->push_back(GRAVITY);
            (*verts)[(i * pixels_x) + j].x = (float)(100 + 2 * j);
            (*verts)[(i * pixels_x) + j].y = (float)(50 + 2 * i);
        }
    }
}

void ClothSim::Particles::update(const double time) {
    this->updateAcc(time);
    this->updateVelo(time);
    this->updatePos(time);
}

void ClothSim::Particles::updateVelo(const double time) {
    for (int64_t i = 0; i < pixels_x * pixels_y; i++)
        (*velos)[i] += ((*accs)[i] * time);
}

void ClothSim::Particles::updatePos(const double time) {
    for (int64_t i = 0; i < pixels_x * pixels_y; i++) {
        (*verts)[i].x += (float)((*velos)[i].x * time);
        (*verts)[i].y += (float)((*velos)[i].y * time);
    }
}

void ClothSim::Particles::calcStruct(const double time) {
    for (int64_t i = 0; i < pixels_y; i++) {
        for (int64_t j = 0; j < pixels_x; j++) {
            if (isValidVert(j + 1, i)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * i) + j + 1].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * i) + j + 1].y);
                struct vect2D delVel = vect2D((*velos)[(pixels_x * i) + j].x - (*velos)[(pixels_x * i) + j + 1].x, (*velos)[(pixels_x * i) + j].y - (*velos)[(pixels_x * i) + j + 1].y);
                struct vect2D acc = normalize(delPos) * (-springConst * ((mag(delPos.x, delPos.y) / 2.0f) - 1.0f));
                acc += ((normalize(delPos) * dotProd(normalize(delPos), (delVel / 2.0f))) * (-dampConst));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j - 1, i)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * i) + j - 1].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * i) + j - 1].y);
                struct vect2D delVel = vect2D((*velos)[(pixels_x * i) + j].x - (*velos)[(pixels_x * i) + j - 1].x, (*velos)[(pixels_x * i) + j].y - (*velos)[(pixels_x * i) + j - 1].y);
                struct vect2D acc = normalize(delPos) * (-springConst * ((mag(delPos.x, delPos.y) / 2.0f) - 1.0f));
                acc += ((normalize(delPos) * dotProd(normalize(delPos), (delVel / 2.0f))) * (-dampConst));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j, i + 1)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i + 1)) + j].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i + 1)) + j].y);
                struct vect2D delVel = vect2D((*velos)[(pixels_x * i) + j].x - (*velos)[(pixels_x * (i + 1)) + j].x, (*velos)[(pixels_x * i) + j].y - (*velos)[(pixels_x * (i + 1)) + j].y);
                struct vect2D acc = normalize(delPos) * (-springConst * ((mag(delPos.x, delPos.y) / 2.0f) - 1.0f));
                acc += ((normalize(delPos) * dotProd(normalize(delPos), (delVel / 2.0f))) * (-dampConst));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j, i - 1)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i - 1)) + j].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i - 1)) + j].y);
                struct vect2D delVel = vect2D((*velos)[(pixels_x * i) + j].x - (*velos)[(pixels_x * (i - 1)) + j].x, (*velos)[(pixels_x * i) + j].y - (*velos)[(pixels_x * (i - 1)) + j].y);
                struct vect2D acc = normalize(delPos) * (-springConst * ((mag(delPos.x, delPos.y) / 2.0f) - 1.0f));
                acc += ((normalize(delPos) * dotProd(normalize(delPos), (delVel / 2.0f))) * (-dampConst));
                (*accs)[(i * pixels_x) + j] += acc;
            }
        }
    }
}

void ClothSim::Particles::calcShear(const double time) {
    for (int64_t i = 0; i < pixels_y; i++) {
        for (int64_t j = 0; j < pixels_x; j++) {
            if (isValidVert(j + 1, i + 1)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i + 1)) + j + 1].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i + 1)) + j + 1].y);
                struct vect2D delVel = vect2D((*velos)[(pixels_x * i) + j].x - (*velos)[(pixels_x * (i + 1)) + j + 1].x, (*velos)[(pixels_x * i) + j].y - (*velos)[(pixels_x * (i + 1)) + j + 1].y);
                struct vect2D acc = normalize(delPos) * (-springConst * ((mag(delPos.x, delPos.y) / sqrt(8.0f)) - 1.0f));
                acc += ((normalize(delPos) * dotProd(normalize(delPos), (delVel / sqrt(8.0f)))) * (-dampConst));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j - 1, i + 1)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i + 1)) + j - 1].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i + 1)) + j - 1].y);
                struct vect2D delVel = vect2D((*velos)[(pixels_x * i) + j].x - (*velos)[(pixels_x * (i + 1)) + j - 1].x, (*velos)[(pixels_x * i) + j].y - (*velos)[(pixels_x * (i + 1)) + j - 1].y);
                struct vect2D acc = normalize(delPos) * (-springConst * ((mag(delPos.x, delPos.y) / sqrt(8.0f)) - 1.0f));
                acc += ((normalize(delPos) * dotProd(normalize(delPos), (delVel / sqrt(8.0f)))) * (-dampConst));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j + 1, i - 1)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i - 1)) + j + 1].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i - 1)) + j + 1].y);
                struct vect2D delVel = vect2D((*velos)[(pixels_x * i) + j].x - (*velos)[(pixels_x * (i - 1)) + j + 1].x, (*velos)[(pixels_x * i) + j].y - (*velos)[(pixels_x * (i - 1)) + j + 1].y);
                struct vect2D acc = normalize(delPos) * (-springConst * ((mag(delPos.x, delPos.y) / sqrt(8.0f)) - 1.0f));
                acc += ((normalize(delPos) * dotProd(normalize(delPos), (delVel / sqrt(8.0f)))) * (-dampConst));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j - 1, i - 1)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i - 1)) + j - 1].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i - 1)) + j - 1].y);
                struct vect2D delVel = vect2D((*velos)[(pixels_x * i) + j].x - (*velos)[(pixels_x * (i - 1)) + j - 1].x, (*velos)[(pixels_x * i) + j].y - (*velos)[(pixels_x * (i - 1)) + j - 1].y);
                struct vect2D acc = normalize(delPos) * (-springConst * ((mag(delPos.x, delPos.y) / sqrt(8.0f)) - 1.0f));
                acc += ((normalize(delPos) * dotProd(normalize(delPos), (delVel / sqrt(8.0f)))) * (-dampConst));
                (*accs)[(i * pixels_x) + j] += acc;
            }
        }
    }
}

void ClothSim::Particles::calcBend(const double time) {
    for (int64_t i = 0; i < pixels_y; i++) {
        for (int64_t j = 0; j < pixels_x; j++) {
            if (isValidVert(j + 2, i)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * i) + j + 2].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * i) + j + 2].y);
                struct vect2D delVel = vect2D((*velos)[(pixels_x * i) + j].x - (*velos)[(pixels_x * i) + j + 2].x, (*velos)[(pixels_x * i) + j].y - (*velos)[(pixels_x * i) + j + 2].y);
                struct vect2D acc = normalize(delPos) * (-springConst * ((mag(delPos.x, delPos.y) / 4.0f) - 1.0f));
                acc += ((normalize(delPos) * dotProd(normalize(delPos), (delVel / 4.0f))) * (-dampConst));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j - 2, i)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * i) + j - 2].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * i) + j - 2].y);
                struct vect2D delVel = vect2D((*velos)[(pixels_x * i) + j].x - (*velos)[(pixels_x * i) + j - 2].x, (*velos)[(pixels_x * i) + j].y - (*velos)[(pixels_x * i) + j - 2].y);
                struct vect2D acc = normalize(delPos) * (-springConst * ((mag(delPos.x, delPos.y) / 4.0f) - 1.0f));
                acc += ((normalize(delPos) * dotProd(normalize(delPos), (delVel / 4.0f))) * (-dampConst));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j, i + 2)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i + 2)) + j].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i + 2)) + j].y);
                struct vect2D delVel = vect2D((*velos)[(pixels_x * i) + j].x - (*velos)[(pixels_x * (i + 2)) + j].x, (*velos)[(pixels_x * i) + j].y - (*velos)[(pixels_x * (i + 2)) + j].y);
                struct vect2D acc = normalize(delPos) * (-springConst * ((mag(delPos.x, delPos.y) / 4.0f) - 1.0f));
                acc += ((normalize(delPos) * dotProd(normalize(delPos), (delVel / 4.0f))) * (-dampConst));
                (*accs)[(i * pixels_x) + j] += acc;
            }
            if (isValidVert(j, i - 2)) {
                struct vect2D delPos = vect2D((*verts)[(pixels_x * i) + j].x - (*verts)[(pixels_x * (i - 2)) + j].x, (*verts)[(pixels_x * i) + j].y - (*verts)[(pixels_x * (i - 2)) + j].y);
                struct vect2D delVel = vect2D((*velos)[(pixels_x * i) + j].x - (*velos)[(pixels_x * (i - 2)) + j].x, (*velos)[(pixels_x * i) + j].y - (*velos)[(pixels_x * (i - 2)) + j].y);
                struct vect2D acc = normalize(delPos) * (-springConst * ((mag(delPos.x, delPos.y) / 4.0f) - 1.0f));
                acc += ((normalize(delPos) * dotProd(normalize(delPos), (delVel / 4.0f))) * (-dampConst));
                (*accs)[(i * pixels_x) + j] += acc;
            }
        }
    }
}