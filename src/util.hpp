#include <bits/stdc++.h>
#include "../inc/SDL.h"

#ifndef UTIL_HPP
#define UTIL_HPP

void log(const std::string &msg);
void info(const std::string &msg);
void error(const std::string &msg);

struct Vector // Vector 2D
{
	float x, y;
	Vector() : x(0.0), y(0.0) {}
	Vector(float val) : x(val), y(val) {}
	Vector(float x, float y) : x(x), y(y) {}
	float distance(const Vector &);
	bool between(const Vector &, const Vector &);
};

// basic Vector operators
std::ostream &
operator<<(std::ostream &, const Vector &);
Vector operator+(const Vector &, const Vector &);
Vector operator-(const Vector &, const Vector &);
Vector operator*(const Vector &, float);
Vector operator/(const Vector &, float);
Vector operator-(const Vector &);
void operator+=(Vector &v1, const Vector &v2);
void operator-=(Vector &v1, const Vector &v2);
void operator*=(Vector &v1, float);
void operator/=(Vector &v1, float);

// basic Vector functions
Vector max(const Vector &, const Vector &);
Vector min(const Vector &, const Vector &);

Vector toInt(const Vector &);

struct Direction
{
	float left, right, up, down;
	Direction() : left(0), right(0), up(0), down(0) {}
	Direction(float l, float r, float u, float d) : left(l), right(r), up(u), down(d) {}
};

struct Color
{
	static SDL_Color white(Uint8 alpha) { return {255, 255, 255, alpha}; }
	static SDL_Color black(Uint8 alpha) { return {0, 0, 0, alpha}; }
	static SDL_Color blue(Uint8 alpha) { return {0, 0, 255, alpha}; }
	static SDL_Color red(Uint8 alpha) { return {255, 0, 0, alpha}; }
	static SDL_Color green(Uint8 alpha) { return {0, 255, 0, alpha}; }
	static SDL_Color violet(Uint8 alpha) { return {238, 130, 238, alpha}; }
	static SDL_Color pink(Uint8 alpha) { return {255, 192, 203, alpha}; }
	static SDL_Color yellow(Uint8 alpha) { return {255, 255, 0, alpha}; }
	static SDL_Color orange(Uint8 alpha) { return {255, 165, 0, alpha}; }
	static SDL_Color cyan(Uint8 alpha) { return {0, 255, 255, alpha}; }
};

struct Rect
{
	static SDL_Rect square(int size) { return {0, 0, size, size}; }
	static bool isCollide(const Vector &, const Vector &, const Vector &, const Vector &);
	static Vector getCenter(const Vector &, const Vector &);
	static SDL_Rect reScale(const Vector &, const Vector &, float);
};

#endif // UTIL_HPP