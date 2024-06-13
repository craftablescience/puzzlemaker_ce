#pragma once

#include <concepts>
#include <type_traits>

template<typename T>
concept CIsArithmetic = std::is_arithmetic_v<T>;

template<CIsArithmetic T>
struct Vec2 {
	T x = 0;
	T y = 0;

	[[nodiscard]] Vec2 operator+(const Vec2& other) const {
		return {this->x + other.x, this->y + other.y};
	}

	[[nodiscard]] Vec2 operator-(const Vec2& other) const {
		return {this->x - other.x, this->y - other.y};
	}

	[[nodiscard]] Vec2 operator+(CIsArithmetic auto other) const {
		return {this->x + other, this->y + other};
	}

	[[nodiscard]] Vec2 operator-(CIsArithmetic auto other) const {
		return {this->x - other, this->y - other};
	}

	[[nodiscard]] bool operator==(const Vec2& other) const {
		return this->x == other.x && this->y == other.y;
	}

	static constexpr Vec2 zero() {
		return {0, 0};
	}
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;

template<CIsArithmetic T>
struct Vec3 {
	T x = 0;
	T y = 0;
	T z = 0;

	[[nodiscard]] Vec3 operator+(const Vec3& other) const {
		return {this->x + other.x, this->y + other.y, this->z + other.z};
	}

	[[nodiscard]] Vec3 operator-(const Vec3& other) const {
		return {this->x - other.x, this->y - other.y, this->z - other.z};
	}

	[[nodiscard]] Vec3 operator+(CIsArithmetic auto other) const {
		return {this->x + other, this->y + other, this->z + other};
	}

	[[nodiscard]] Vec3 operator-(CIsArithmetic auto other) const {
		return {this->x - other, this->y - other, this->z - other};
	}

	[[nodiscard]] bool operator==(const Vec3& other) const {
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}

	static constexpr Vec3 zero() {
		return {0, 0, 0};
	}
};

using Vec3i = Vec3<int>;
using Vec3f = Vec3<float>;
