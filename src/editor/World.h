#pragma once

#include <string>
#include <vector>

#include "Octree.h"

constexpr int MAX_CHAMBER_SIZE = 32768;
constexpr int DEFAULT_RESOLUTION = 128;

class VoxelData {
	std::string texture;
};

#pragma pack(push, 1)
struct Vertex {
	Vertex(Vec3f pos_, Vec2f uv_)
			: pos(pos_)
			, uv(uv_) {}

	Vec3f pos;
	Vec2f uv;
};
#pragma pack(pop)

class World {
public:
	World()
		: chamber(MAX_CHAMBER_SIZE)
		, editResolution(DEFAULT_RESOLUTION) {
		// 8*128 x
		// 6*128 y
		// 4*128 z
	}

	[[nodiscard]] std::vector<Vertex> render() {
		std::vector<Vertex> vertices;
		this->render(vertices, this->chamber.root());
		return vertices;
	}

private:
	Octree<VoxelData> chamber;
	int editResolution;

	// NOLINTNEXTLINE(*-no-recursion)
	void render(std::vector<Vertex>& vertices, const std::unique_ptr<Octree<VoxelData>::Node>& node) {
		for (auto& child : this->chamber.root()->children()) {
			if (!child) {
				continue;
			}
			if (child->hasChildren()) {
				this->render(vertices, child);
			} else {
				World::addVoxelToVertices(vertices, node->position(), node->halfSize());
			}
		}
	}

	static void addVoxelToVertices(std::vector<Vertex>& vertices, Vec3i center, int halfSize) {
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z - halfSize)}, {0.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z - halfSize)}, {1.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z - halfSize)}, {1.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z - halfSize)}, {1.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z - halfSize)}, {0.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z - halfSize)}, {0.0f, 0.0f}});

		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z + halfSize)}, {0.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z + halfSize)}, {1.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z + halfSize)}, {1.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z + halfSize)}, {1.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z + halfSize)}, {0.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z + halfSize)}, {0.0f, 0.0f}});

		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z + halfSize)}, {1.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z - halfSize)}, {1.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z - halfSize)}, {0.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z - halfSize)}, {0.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z + halfSize)}, {0.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z + halfSize)}, {1.0f, 0.0f}});

		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z + halfSize)}, {1.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z - halfSize)}, {1.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z - halfSize)}, {0.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z - halfSize)}, {0.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z + halfSize)}, {0.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z + halfSize)}, {1.0f, 0.0f}});

		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z - halfSize)}, {0.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z - halfSize)}, {1.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z + halfSize)}, {1.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z + halfSize)}, {1.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z + halfSize)}, {0.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y - halfSize), static_cast<float>(center.z - halfSize)}, {0.0f, 1.0f}});

		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z - halfSize)}, {0.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z - halfSize)}, {1.0f, 1.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z + halfSize)}, {1.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x + halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z + halfSize)}, {1.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z + halfSize)}, {0.0f, 0.0f}});
		vertices.push_back({{static_cast<float>(center.x - halfSize), static_cast<float>(center.y + halfSize), static_cast<float>(center.z - halfSize)}, {0.0f, 1.0f}});
	}
};
