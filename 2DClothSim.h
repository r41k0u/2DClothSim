// 2DClothSim.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include "SDL.h"
#include <iostream>
#include <vector>

namespace ClothSim {

	SDL_Window* window;
	SDL_Renderer* renderer;

	class Cloth {

	private:
		int64_t pixels_x, pixels_y;
		bool init;
		std::vector<SDL_FPoint> *verts;

		void initVerts();

	public:
		// Constructor
		Cloth(int x = 100, int y = 100) : pixels_x(x), pixels_y(y), init(false), verts(nullptr) { initVerts(); }

		// Getters
		int64_t getPixelsX() { return pixels_x; }
		int64_t getPixelsY() { return pixels_x; }
		std::vector<SDL_FPoint>* getVerts() { return verts; }
	};

	void waitForExit();
}
