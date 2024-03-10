#include "Utility.hpp"

Vector left_vec(-1, 0);
Vector right_vec(1, 0);
Vector up_vec(0, -1);
Vector down_vec(0, 1);

std::ostream &operator<<(std::ostream &out, const Vector &v)
{
	out << '(' << v.x << ", " << v.y << ')' << '\n';
	return out;
}

Vector operator+(const Vector &v1, const Vector &v2)
{
	return Vector(v1.x + v2.x, v1.y + v2.y);
}

Vector operator-(const Vector &v1, const Vector &v2)
{
	return Vector(v1.x - v2.x, v1.y - v2.y);
}

Vector operator*(const Vector &v, float k)
{
	return Vector(v.x * k, v.y * k);
}

Vector operator/(const Vector &v, float k)
{
	return Vector(v.x / k, v.y / k);
}

Vector operator-(const Vector &v)
{
	return Vector(-v.x, -v.y);
}

void operator+=(Vector &v1, const Vector &v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
}

void operator-=(Vector &v1, const Vector &v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
}

void operator*=(Vector &v, float k)
{
	v.x *= k;
	v.y *= k;
}

void operator/=(Vector &v, float k)
{
	v.x /= k;
	v.y /= k;
}

// basic Vector functions
Vector max(const Vector &v1, const Vector &v2)
{
	return {std::max(v1.x, v2.x), std::max(v1.y, v2.y)};
}

Vector min(const Vector &v1, const Vector &v2)
{
	return {std::min(v1.x, v2.x), std::min(v1.y, v2.y)};
}

Vector toInt(const Vector &v)
{
	return Vector(int(v.x), int(v.y));
}

float Vector::distance(const Vector &v)
{
	float dx = v.x - x;
	float dy = v.y - y;
	return sqrt(dx * dx + dy * dy);
}

bool Vector::between(const Vector &v1, const Vector &v2)
{
	return x >= v1.x && x <= v2.x && y >= v1.y && y <= v2.y;
}

bool Rect::isCollide(const Vector &pos1st, const Vector &size1st, const Vector &pos2nd, const Vector &size2nd)
{
	float c_x = abs(pos1st.x - pos2nd.x) - size1st.x / 2 - size2nd.x / 2;
	float c_y = abs(pos1st.y - pos2nd.y) - size1st.y / 2 - size2nd.y / 2;
	return c_x <= 0 && c_y <= 0;
}

Vector Rect::getCenter(const Vector &pos, const Vector &size)
{
	return pos + size / 2;
}