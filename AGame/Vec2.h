#pragma once
#include <iostream>
#include "AEEngine.h"

template <typename T>
struct Vec2 {
	T x{ 0 };
	T y{ 0 };
	Vec2<T>() = default;
	Vec2<T>(const T& xx, const T& yy) : x{ xx }, y{ yy } {}
	Vec2<T> operator+(const Vec2<T>& rhs) {
		return { x + rhs.x,y + rhs.y };
	}
	Vec2<T>& operator+=(const Vec2<T>& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vec2<T> operator-(const Vec2<T>& rhs) {
		return { x - rhs.x,y - rhs.y };
	}
	Vec2<T>& operator-=(const Vec2<T>& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	// dot product
	T operator*(const Vec2<T>& rhs) {
		return x * rhs.x + y * rhs.y;
	}
	// scale
	Vec2<T> operator*(const float& scale) const {
		return { x * scale, y * scale };
	}
	// conversion constructor
	Vec2<T>(const AEVec2& rhs) : x{rhs.x}, y{rhs.y} {}
	// conversion operator
	operator AEVec2() const { return { x,y }; }
	// vector functions
	double Length() const {
		return sqrt(x * x + y * y);
	}
	Vec2<T>& NormalizeSelf() {
		double length = Length();
		x /= (T)length;
		y /= (T)length;
		return *this;
	}
	Vec2<T> Normalize() const {
		double length = Length();
		return { x / (T)length,y / (T)length };
	}
	Vec2<T> RotateAngle(const float& rad) const {
		return { x * cos(rad) + y * sin(rad),
				x * -sin(rad) + y * cos(rad) };
	}
	void Print() {
		std::cout << "(" << x << ", " << y << ")" << std::endl;
	}
};

typedef Vec2<float>		Vec2f;
typedef Vec2<int>		Vec2i;
typedef Vec2<double>	Vec2d;