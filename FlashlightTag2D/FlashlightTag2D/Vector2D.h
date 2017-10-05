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

	Vector2D operator-()
	{
		Vector2D newVec(*this);
		newVec.x = -newVec.x;
		newVec.y = -newVec.y;

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

	Vector2D operator/(const T& scalar)
	{
		Vector2D newVec(*this);
		newVec.x /= scalar;
		newVec.y /= scalar;

		return newVec;
	}

	void operator=(const Vector2D& rhs)
	{
		this->x = rhs.x;
		this->y = rhs.y;
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

	std::unique_ptr<T[]> GetPtrToFlattenedData()
	{
		std::unique_ptr<T[]> pflat(new T[2]);
		T* flat = new T[2] {x, y};
		//std::cout << "Flattened ****************************" << std::endl;
		

		//std::cout << "****************************" << std::endl;
		return std::unique_ptr<T[]>(flat);
	}
};