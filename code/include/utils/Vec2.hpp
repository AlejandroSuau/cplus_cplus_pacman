#pragma once

#include <cmath>

template<typename T>
class Vec2 {
    static_assert(std::is_default_constructible<T>::value, "T should be default constructible");
public:
    T x, y;

    Vec2() : x(T()), y(T()) {}
    Vec2(T x_, T y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(T scalar) const;
    Vec2 operator/(T scalar) const;

    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(const Vec2& other);
    Vec2& operator*=(T scalar);
    Vec2& operator/=(T scalar);

    bool operator!=(const Vec2& other) const;
    bool operator==(const Vec2& other) const;

    T Length() const;
    T LengthSquared() const;
    Vec2 Normalized() const;
    T Dot(const Vec2& other) const;
};

template<typename T>
Vec2<T> Vec2<T>::operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}


template<typename T>
Vec2<T> Vec2<T>::operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}

template<typename T>
Vec2<T> Vec2<T>::operator*(T scalar) const {
    return Vec2(x * scalar, y * scalar);
}

template<typename T>
Vec2<T> Vec2<T>::operator/(T scalar) const {
    return Vec2(x / scalar, y / scalar);
}

template<typename T>
Vec2<T>& Vec2<T>::operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator*=(const Vec2& other) {
    x *= other.x;
    y *= other.y;
    return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator/=(T scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

template<typename T>
T Vec2<T>::Length() const {
    return static_cast<T>(std::sqrt(x * x + y * y));
}

template<typename T>
T Vec2<T>::LengthSquared() const {
    return x * x + y * y;
}

template<typename T>
bool Vec2<T>::operator!=(const Vec2& other) const {
    return (x != other.x || y != other.y);
}

template<typename T>
bool Vec2<T>::operator==(const Vec2& other) const {
    return (x == other.x && y == other.y);
}

template<typename T>
Vec2<T> Vec2<T>::Normalized() const {
    T length = Length();
    if (length != 0) {
        return Vec2(x / length, y / length);
    }
    return Vec2(0, 0);
}

template<typename T>
T Vec2<T>::Dot(const Vec2& other) const {
    return x * other.x + y * other.y;
}
