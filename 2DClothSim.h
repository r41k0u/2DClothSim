// 2DClothSim.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include "SDL.h"
#include <iostream>
#include <vector>
#include <math.h>

#define GRAVITY vect2D(.0f, 9.8f)

namespace ClothSim {

	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static double time;

	struct vect2D {
		double x;
		double y;

		vect2D(double inp_x, double inp_y) { x = inp_x; y = inp_y; }

		vect2D operator*(double scalar) const { return vect2D(x * scalar, y * scalar); }
		vect2D operator/(double scalar) const { return vect2D(x / scalar, y / scalar); }
		struct vect2D& operator*=(double scalar) {
			x *= scalar;
			y *= scalar;
			return *this;
		}
		struct vect2D& operator/=(double scalar) {
			if (scalar != 0.0f) {
				x /= scalar;
				y /= scalar;
			}
			return *this;
		}
		struct vect2D& operator+=(const vect2D& v2) {
			x += v2.x;
			y += v2.y;
			return *this;
		}
		struct vect2D& operator-=(const vect2D& v2) {
			x -= v2.x;
			y -= v2.y;
			return *this;
		}
	};

	class Particles {
	public:
		int64_t pixels_x, pixels_y;
		std::vector<SDL_FPoint>* verts;
		std::vector<struct vect2D>* velos, * accs;
		double springConst, dampConst, vertMass;

		const struct vect2D dirX = vect2D(1.0f, .0f), dirY = vect2D(.0f, 1.0f), dirNE = vect2D(sqrtf(0.5f), -sqrtf(0.5f)), dirSE = vect2D(sqrtf(0.5f), sqrtf(0.5f));

		void initVerts();
		Particles(int64_t x, int64_t y, float spring = 1000.0f) : pixels_x(x), pixels_y(y), verts(nullptr), velos(nullptr), springConst(spring), dampConst(.2f), vertMass(1.0f) { initVerts(); }
		
		void update(const double time);
		virtual void updateAcc(const double time) = 0;
		void updateVelo(const double time);
		void updatePos(const double time);

		void calcStruct(const double time);
		void calcShear(const double time);
		void calcBend(const double time);

		inline bool isValidVert(int64_t x, int64_t y) { return ((-1 < x) && (x < pixels_x) && (-1 < y) && (y < pixels_y)); }
		inline double mag(double x, double y) { return sqrt(pow(x, 2) + pow(y, 2)); }
		inline struct vect2D normalize(const struct vect2D& vec) { return (vec / mag(vec.x, vec.y)); }
		inline double dotProd(struct vect2D& v1, struct vect2D& v2) { return ((v1.x * v2.x) + (v1.y + v2.y)); }

		// Getters
		int64_t getPixelsX() { return pixels_x; }
		int64_t getPixelsY() { return pixels_y; }
		std::vector<SDL_FPoint>* getVerts() { return verts; }
	};

	class Cloth : public Particles {
	public:
		// Constructor
		Cloth(int64_t x = 100, int64_t y = 100) : Particles(x, y) {}

		void updateAcc(const double time);
	};

	class OneDSystem : public Particles {
	private:
		void updateAcc(const double time);
	public:
		// Constructor
		OneDSystem(int64_t x = 1, int64_t y = 3, float spring = 10.0f) : Particles(x, y, spring) {}
	};
}
