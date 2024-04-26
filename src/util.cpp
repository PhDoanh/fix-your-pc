#include "util.hpp"
#include "Game.hpp"

Vec2D left_vec(-1, 0);
Vec2D right_vec(1, 0);
Vec2D up_vec(0, -1);
Vec2D down_vec(0, 1);

void drawLine(const Vec2D &pos1, const Vec2D &pos2, const SDL_Color &color)
{
	SDL_SetRenderDrawColor(Game::renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLineF(Game::renderer, pos1.x, pos1.y, pos2.x, pos2.y);
}

void drawRect(const Vec2D &pos, const Vec2D &size, const SDL_Color &color)
{
	SDL_SetRenderDrawColor(Game::renderer, color.r, color.g, color.b, color.a);
	SDL_FRect rect = {pos.x, pos.y, size.x, size.y};
	SDL_RenderDrawRectF(Game::renderer, &rect);
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

bool Rect::isCollide(const Vec2D &v1pos, const Vec2D &v1size, const Vec2D &v2pos, const Vec2D &v2size)
{
	float c_x = abs(v1pos.x - v2pos.x) - v1size.x / 2 - v2size.x / 2;
	float c_y = abs(v1pos.y - v2pos.y) - v1size.y / 2 - v2size.y / 2;
	return c_x <= 0 && c_y <= 0;
}

Uint32 Rect::get_pixel(SDL_Surface *surface, const int &x, const int &y)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	return pixels[(y * surface->w) + x];
}

bool Rect::isPixelCollide(SDL_Texture *texture1, SDL_Rect rect1, SDL_Texture *texture2, SDL_Rect rect2)
{
	if (!SDL_HasIntersection(&rect1, &rect2))
		return false;

	SDL_Surface *surface1 = SDL_CreateRGBSurfaceWithFormat(0, rect1.w, rect1.h, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_Surface *surface2 = SDL_CreateRGBSurfaceWithFormat(0, rect2.w, rect2.h, 32, SDL_PIXELFORMAT_RGBA32);

	SDL_RenderReadPixels(Game::renderer, &rect1, SDL_PIXELFORMAT_RGBA32, surface1->pixels, surface1->pitch);
	SDL_RenderReadPixels(Game::renderer, &rect2, SDL_PIXELFORMAT_RGBA32, surface2->pixels, surface2->pitch);

	SDL_Rect intersection;
	SDL_IntersectRect(&rect1, &rect2, &intersection);

	for (int y = intersection.y; y < intersection.y + intersection.h; y++)
	{
		for (int x = intersection.x; x < intersection.x + intersection.w; x++)
		{
			Uint32 pixel1 = get_pixel(surface1, x - rect1.x, y - rect1.y);
			Uint32 pixel2 = get_pixel(surface2, x - rect2.x, y - rect2.y);

			Uint8 r, g, b, a;
			SDL_GetRGBA(pixel1, surface1->format, &r, &g, &b, &a);
			bool pixel1_opaque = a > 0;

			SDL_GetRGBA(pixel2, surface2->format, &r, &g, &b, &a);
			bool pixel2_opaque = a > 0;

			if (pixel1_opaque && pixel2_opaque)
			{
				SDL_FreeSurface(surface1);
				SDL_FreeSurface(surface2);
				return true;
			}
		}
	}

	SDL_FreeSurface(surface1);
	SDL_FreeSurface(surface2);
	return false;
}

Vec2D Rect::getCenter(const Vec2D &pos, const Vec2D &size)
{
	return pos + size / 2;
}

SDL_FRect Rect::reScale(const Vec2D &pos, const Vec2D &size, float scale)
{
	SDL_FRect new_rect;
	new_rect.x = pos.x + (1 - scale) * size.x / 2.0;
	new_rect.y = pos.y + (1 - scale) * size.y / 2.0;
	new_rect.w = scale * size.x;
	new_rect.h = scale * size.y;
	return new_rect;
}
