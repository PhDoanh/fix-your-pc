#include "util.hpp"

Vec2D left_vec(-1, 0);
Vec2D right_vec(1, 0);
Vec2D up_vec(0, -1);
Vec2D down_vec(0, 1);

void log(const std::string &msg)
{
	std::clog << "  log| " << msg << '\n';
}

void info(const std::string &msg)
{
	std::cout << " info| " << msg << '\n';
}

void error(const std::string &msg)
{
	std::cerr << "error| " << msg << '\n';
	exit(1);
}

std::ostream &operator<<(std::ostream &out, const Vec2D &v)
{
	out << '(' << v.x << ", " << v.y << ')' << '\n';
	return out;
}

Vec2D operator+(const Vec2D &v1, const Vec2D &v2)
{
	return Vec2D(v1.x + v2.x, v1.y + v2.y);
}

Vec2D operator-(const Vec2D &v1, const Vec2D &v2)
{
	return Vec2D(v1.x - v2.x, v1.y - v2.y);
}

Vec2D operator*(const Vec2D &v, float k)
{
	return Vec2D(v.x * k, v.y * k);
}

Vec2D operator/(const Vec2D &v, float k)
{
	return Vec2D(v.x / k, v.y / k);
}

Vec2D operator-(const Vec2D &v)
{
	return Vec2D(-v.x, -v.y);
}

void operator+=(Vec2D &v1, const Vec2D &v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
}

void operator-=(Vec2D &v1, const Vec2D &v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
}

void operator*=(Vec2D &v, float k)
{
	v.x *= k;
	v.y *= k;
}

void operator/=(Vec2D &v, float k)
{
	v.x /= k;
	v.y /= k;
}

Vec2D max(const Vec2D &v1, const Vec2D &v2)
{
	return {std::max(v1.x, v2.x), std::max(v1.y, v2.y)};
}

Vec2D min(const Vec2D &v1, const Vec2D &v2)
{
	return {std::min(v1.x, v2.x), std::min(v1.y, v2.y)};
}

Vec2D toInt(const Vec2D &v)
{
	return Vec2D(int(v.x), int(v.y));
}

float Vec2D::distance(const Vec2D &v)
{
	float dx = v.x - x, dy = v.y - y;
	return sqrt(dx * dx + dy * dy);
}

bool Vec2D::between(const Vec2D &v1, const Vec2D &v2)
{
	return x >= v1.x && x <= v2.x && y >= v1.y && y <= v2.y;
}

bool Rect::isCollide(const Vec2D &v1pos, const Vec2D &v1size, const Vec2D &v2pos, const Vec2D &v2size)
{
	float c_x = abs(v1pos.x - v2pos.x) - v1size.x / 2 - v2size.x / 2;
	float c_y = abs(v1pos.y - v2pos.y) - v1size.y / 2 - v2size.y / 2;
	return c_x <= 0 && c_y <= 0;
}

Vec2D Rect::getCenter(const Vec2D &pos, const Vec2D &size)
{
	return pos + size / 2;
}

SDL_Rect Rect::reScale(const Vec2D &pos, const Vec2D &size, float scale)
{
	SDL_Rect new_rect;
	new_rect.x = pos.x + (1 - scale) * size.x / 2;
	new_rect.y = pos.y + (1 - scale) * size.y / 2;
	new_rect.w = scale * size.x;
	new_rect.h = scale * size.y;
	return new_rect;
}
