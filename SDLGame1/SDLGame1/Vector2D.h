#pragma once
#include <cmath>

template <class T>
class Vector2D
{
public:
	T x, y;

	Vector2D(T p_x, T p_y)
		: x(p_x),
		  y(p_y) { }

	Vector2D(const Vector2D& p_vec)
		: x(p_vec.x),
		  y(p_vec.y) { }

	Vector2D operator+(const Vector2D& rhs)
	{
		Vector2D newVec(*this);
		newVec.x += rhs.x;
		newVec.y += rhs.y;
		
		return newVec;
	}

	Vector2D operator-(const Vector2D& rhs)
	{
		Vector2D newVec(*this);
		newVec.x -= rhs.x;
		newVec.y -= rhs.y;

		return newVec;
	}

	void operator+=(const Vector2D& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}

	void operator-=(const Vector2D& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}

	Vector2D operator*(const T& scalar)
	{
		Vector2D newVec(*this);
		newVec.x *= scalar;
		newVec.y *= scalar;

		return newVec;
	}

	T Dot(const Vector2D& rhs)
	{
		return (x * rhs.x) + (y * rhs.y);
	}

	T Length()
	{
		return std::sqrt((x * x) + (y * y));
	}

	Vector2D Normalize()
	{
		Vector2D newVec(*this);
		T length = newVec.Length();

		newVec.x /= length;
		newVec.y /= length;

		return newVec;
	}
};