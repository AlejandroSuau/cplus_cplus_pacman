#pragma once

class Vec2 {
public:
    int x, y;

    Vec2() : x(0), y(0) {}
    Vec2(int x_, int y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(int scalar) const;
    Vec2 operator/(int scalar) const;

    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(const Vec2& other);
    Vec2& operator*=(int scalar);
    Vec2& operator/=(int scalar);

    bool operator!=(const Vec2& other) const;
    bool operator==(const Vec2& other) const;

    int Length() const;
    int LengthSquared() const;
    Vec2 Normalized() const;
    int Dot(const Vec2& other) const;
};
