#pragma once

#include <array>
#include <memory>
#include <variant>
#include <utility>

#include <sourcepp/math/Vector.h>

using namespace sourcepp::math;

template<typename D>
class Octree {
public:
	class Node {
	public:
		explicit Node(Vec3i position, int halfSize)
			: data_(D{})
			, position_(position)
			, halfSize_(halfSize) {}

		[[nodiscard]] bool hasChildren() const {
			return !this->isLeaf;
		}

		/// Split voxel into 8 subvoxels
		void subdivide() {
			this->data_ = std::array<std::unique_ptr<Node>, 8>{};
			this->isLeaf = false;
		}

		/// Merge 8 subvoxels into one voxel
		void merge(D data) {
			this->data_ = std::move(data);
			this->isLeaf = true;
		}

		[[nodiscard]] const D& data() const {
			return std::get<D>(this->data_);
		}

		[[nodiscard]] const std::array<std::unique_ptr<Node>, 8>& children() const {
			return std::get<std::array<std::unique_ptr<Node>, 8>>(this->data_);
		}

		[[nodiscard]] std::unique_ptr<Node>& child(int index) {
			auto& node = std::get<std::array<std::unique_ptr<Node>, 8>>(this->data_).at(index);
			if (!node) {
				node = std::make_unique<Node>(this->getPositionFromIndex(index), this->halfSize() / 2);
			}
			return node;
		}

		[[nodiscard]] const std::unique_ptr<Node>& child(int index) const {
			return this->children().at(index);
		}

		/// The center of the voxel
		[[nodiscard]] Vec3i position() const {
			return this->position_;
		}

		[[nodiscard]] int halfSize() const {
			return this->halfSize_;
		}

		[[nodiscard]] bool isPositionOnBounds(Vec3i position) {
			auto minX = this->position().x - this->halfSize();
			auto maxX = this->position().x + this->halfSize();
			auto minY = this->position().y - this->halfSize();
			auto maxY = this->position().y + this->halfSize();
			auto minZ = this->position().z - this->halfSize();
			auto maxZ = this->position().z + this->halfSize();
			return position.x == minX || position.x == maxX &&
				   position.y == minY || position.y == maxY &&
				   position.z == minZ || position.z == maxZ;
		}

		[[nodiscard]] bool isPositionWithinBounds(Vec3i position) const {
			auto modifiedPosition = position - this->position();
			return modifiedPosition.x > this->halfSize() - 1 || modifiedPosition.x < -this->halfSize() + 1 ||
			       modifiedPosition.y > this->halfSize() - 1 || modifiedPosition.y < -this->halfSize() + 1 ||
			       modifiedPosition.z > this->halfSize() - 1 || modifiedPosition.z < -this->halfSize() + 1;
		}

		[[nodiscard]] Vec3i getPositionFromIndex(int index) {
			Vec3i newPos = this->position();
			const auto newPosDelta = this->halfSize() / 2;

			if ((index & 0b100) == 0b100) {
				newPos.x += newPosDelta;
			} else {
				newPos.x -= newPosDelta;
			}
			if ((index & 0b010) == 0b010) {
				newPos.y += newPosDelta;
			} else {
				newPos.y -= newPosDelta;
			}
			if ((index & 0b001) == 0b001) {
				newPos.z += newPosDelta;
			} else {
				newPos.z -= newPosDelta;
			}
			return newPos;
		}

		// Morton ordering
		[[nodiscard]] int getIndexFromPosition(Vec3i queryPosition) {
			int index = 0;
			index |= queryPosition.x > this->position().x ? 4 : 0;
			index |= queryPosition.y > this->position().y ? 2 : 0;
			index |= queryPosition.z > this->position().z ? 1 : 0;
			return index;
		}

	private:
		std::variant<D, std::array<std::unique_ptr<Node>, 8>> data_;
		// true - holds voxel, false - holds children
		bool isLeaf = true;
		// not centered
		Vec3i position_;
		// half the size of the voxel (radius?)
		int halfSize_;
	};

	explicit Octree(int size)
		: root_(std::make_unique<Node>(Vec3i::zero(), size / 2)) {}

	/// Set voxel data in the octree
	[[nodiscard]] bool set(Vec3i position, const D& data, bool forceMerge = false) {
		return this->set(this->root_, position, data, forceMerge);
	}

	/// Get an existing voxel from the octree. Returns nullptr if the voxel doesn't exist
	[[nodiscard]] std::unique_ptr<Node>& get(Vec3i position) {
		return this->get(this->root_, position);
	}

	[[nodiscard]] bool exists(Vec3i position) const {
		return this->exists(this->root_, position);
	}

	void simplify() {
		// todo
	}

	[[nodiscard]] std::unique_ptr<Node>& root() {
		return this->root_;
	}

	[[nodiscard]] const std::unique_ptr<Node>& root() const {
		return this->root_;
	}

private:
	// NOLINTNEXTLINE(*-no-recursion)
	[[nodiscard]] bool set(std::unique_ptr<Node>& node, Vec3i position, const D& data, bool forceMerge = false) {
		if (node->isPositionOnBounds(position)) {
			return false;
		}
		if (node->position() == position) {
			if (node->hasChildren() && !forceMerge) {
				return false;
			}
			node->merge(data);
			return true;
		}
		if (!node->hasChildren()) {
			node->subdivide();
		}
		return this->set(node->child(node->getIndexFromPosition(position)), position, data);
	}

	// NOLINTNEXTLINE(*-no-recursion)
	[[nodiscard]] std::unique_ptr<Node>& get(std::unique_ptr<Node>& node, Vec3i position) {
		if (node->isPositionOnBounds(position)) {
			static std::unique_ptr<Node> INVALID = nullptr;
			return INVALID;
		}
		if (!node->hasChildren()) {
			return node;
		}
		auto& child = node->child(node->getIndexFromPosition(position));
		return this->get(child, position);
	}

	// NOLINTNEXTLINE(*-no-recursion)
	[[nodiscard]] bool exists(const std::unique_ptr<Node>& node, Vec3i position) const {
		if (node->isPositionOnBounds(position)) {
			return false;
		}
		if (node->position() == position) {
			return true;
		}
		if (!node->hasChildren()) {
			return false;
		}
		auto& child = node->children()[node->getIndexFromPosition(position)];
		if (!child) {
			return false;
		}
		return this->exists(child, position);
	}

	std::unique_ptr<Node> root_;
};
