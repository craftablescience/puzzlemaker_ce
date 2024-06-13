#include <gtest/gtest.h>

#include <editor/Octree.h>

TEST(Octree, exists) {
	Octree<int> octree(16);

	ASSERT_TRUE(octree.exists(Vec3i::zero()));
	ASSERT_FALSE(octree.exists({0, 0, 1}));
}

TEST(Octree, level1) {
	Octree<int> octree(16);

	ASSERT_TRUE(octree.set(Vec3i::zero(), 42));
	ASSERT_EQ(octree.get({0, 0, 0})->data(), 42);
}

TEST(Octree, level2) {
	Octree<int> octree(16);

	ASSERT_TRUE(octree.set(Vec3i::zero(), 42));
	ASSERT_EQ(octree.get({0, 0, 0})->data(), 42);

	ASSERT_TRUE(octree.set({4, 4, 4}, 42));
	ASSERT_EQ(octree.get({4, 4, 4})->data(), 42);
}

TEST(Octree, level3) {
	Octree<int> octree(16);

	ASSERT_TRUE(octree.set(Vec3i::zero(), 42));
	ASSERT_EQ(octree.get({0, 0, 0})->data(), 42);

	ASSERT_FALSE(octree.set({4, 4, 6}, 42));
	ASSERT_FALSE(octree.set({2, 4, 6}, 42));

	ASSERT_TRUE(octree.set({1, 3, 5}, 42));
	ASSERT_EQ(octree.get({1, 3, 5})->data(), 42);
}
