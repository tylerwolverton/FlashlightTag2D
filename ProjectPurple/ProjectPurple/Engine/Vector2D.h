#pragma once
#include <cmath>
#include <memory>

template <class T>
class Vector2D
{
public:
    T x, y;

    Vector2D()
        : x(0),
          y(0) { }

    Vector2D(T p_x, T p_y)
        : x(p_x),
          y(p_y) { }

    Vector2D(const Vector2D& p_vec)
        : x(p_vec.x),
          y(p_vec.y) { }

    Vector2D operator+(const Vector2D& rhs) const
    {
        Vector2D newVec(*this);
        newVec.x += rhs.x;
        newVec.y += rhs.y;
        
        return newVec;
    }

    Vector2D operator-(const Vector2D& rhs) const
    {
        Vector2D newVec(*this);
        newVec.x -= rhs.x;
        newVec.y -= rhs.y;

        return newVec;
    }

    Vector2D operator-() const
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

    Vector2D operator*(const T& scalar) const
    {
        Vector2D newVec(*this);
        newVec.x *= scalar;
        newVec.y *= scalar;

        return newVec;
    }

    Vector2D operator/(const T& scalar) const
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

    bool operator==(const Vector2D& rhs)
    {
        return (this->x == rhs.x && this->y == rhs.y);
    }

    bool operator!=(const Vector2D& rhs)
    {
        return !(this->x == rhs.x && this->y == rhs.y);
    }

    T Dot(const Vector2D& rhs) const
    {
        return (x * rhs.x) + (y * rhs.y);
    }

    T Length() const
    {
        return std::sqrt((x * x) + (y * y));
    }

    Vector2D Normalize() const
    {
        Vector2D newVec(*this);
        T length = newVec.Length();

        if (length != 0)
        {
            newVec.x /= length;
            newVec.y /= length;
        }

        return newVec;
    }

    std::unique_ptr<T[]> GetPtrToFlattenedData()
    {
        std::unique_ptr<T[]> pflat(new T[2]);
        T* flat = new T[2] {x, y};

        return std::unique_ptr<T[]>(flat);
    }
};

template <class T>
Vector2D<T> operator-(const Vector2D<T>& lhs, const Vector2D<T>& rhs)
{
    Vector2D<T> newVec(lhs);
    newVec.x -= rhs.x;
    newVec.y -= rhs.y;

    return newVec;
}

template <class T>
Vector2D<T> operator+(const Vector2D<T>& lhs, const Vector2D<T>& rhs)
{
    Vector2D<T> newVec(lhs);
    newVec.x += rhs.x;
    newVec.y += rhs.y;

    return newVec;
}

template <class T>
Vector2D<T> operator*(const T& lhs, const Vector2D<T>& rhs)
{
    Vector2D<T> newVec(rhs);
    newVec.x *= lhs;
    newVec.y *= lhs;

    return newVec;
}