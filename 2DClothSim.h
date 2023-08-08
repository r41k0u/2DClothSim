// 2DClothSim.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include "SDL.h"
#include <iostream>
#include <vector>
#include <math.h>

namespace ClothSim {

	SDL_Window* window;
	SDL_Renderer* renderer;
	double time;

	struct vect2D {
		float x;
		float y;

		vect2D(float inp_x, float inp_y) { x = inp_x; y = inp_y; }

		vect2D operator*(float scalar) const { return vect2D(x * scalar, y * scalar); }
		vect2D operator/(float scalar) const { return vect2D(x / scalar, y / scalar); }
		struct vect2D& operator*=(float scalar) {
			x *= scalar;
			y *= scalar;
			return *this;
		}
		struct vect2D& operator/=(float scalar) {
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

	class Cloth {

	private:
		int64_t pixels_x, pixels_y;
		bool init;
		std::vector<SDL_FPoint> *verts;
		std::vector<struct vect2D> *velos, *accs;
		float springConst, dampConst, vertMass;

		const struct vect2D dirX = vect2D(1.0f, .0f), dirY = vect2D(.0f, 1.0f), dirNE = vect2D(sqrtf(0.5f), -sqrtf(0.5f)), dirSE = vect2D(sqrtf(0.5f), sqrtf(0.5f));

		void initVerts();

	public:
		// Constructor
		Cloth(int x = 100, int y = 100) : pixels_x(x), pixels_y(y), init(false), verts(nullptr), velos(nullptr), springConst(1000.0f), dampConst(.2f), vertMass(1.0f) { initVerts(); }

		void updateAcc(const float time);
		void updateVelo(const float time);
		void updatePos(const float time);

		void calcStruct(const float time);
		void calcShear(const float time);
		void calcBend(const float time);

		inline bool isValidVert(int64_t x, int64_t y) { return ((-1 < x) && (x < pixels_x) && (-1 < y) && (y < pixels_y)); }
		inline float mag(float x, float y) { return sqrt(pow(x, 2) + pow(y, 2)); }
		inline struct vect2D normalize(const struct vect2D& vec) { return (vec / mag(vec.x, vec.y)); }
		inline float dotProd(struct vect2D& v1, struct vect2D& v2) { return ((v1.x * v2.x) + (v1.y + v2.y)); }

		// Getters
		int64_t getPixelsX() { return pixels_x; }
		int64_t getPixelsY() { return pixels_x; }
		std::vector<SDL_FPoint>* getVerts() { return verts; }
	};

	void update(Cloth* cloth, const float time);
}
