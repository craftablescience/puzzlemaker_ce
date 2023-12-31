#pragma once

#include <concepts>

template<std::integral T>
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

	[[nodiscard]] Vec3 operator+(std::integral auto other) const {
		return {this->x + other, this->y + other, this->z + other};
	}

	[[nodiscard]] Vec3 operator-(std::integral auto other) const {
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
