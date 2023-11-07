#include "CppSlippi/Utils/PortArray.h"

#include <stdexcept>

#include <gtest/gtest.h>

#include "CppSlippi/Model/Enums/Port.h"


namespace cpp_slippi::utils {

TEST(PortArrayTest, TestInitializationAndEquals) {
	PortArray<int> p = {1, 2, 3, 4};

	EXPECT_EQ(p, (PortArray<int>{1, 2, 3, 4}));
}

TEST(PortArrayTest, TestAt) {
	PortArray<int> p = {1, 2, 3, 4};

	EXPECT_EQ(p.at(Port::P1), 1);
	EXPECT_EQ(p.at(Port::P2), 2);
	EXPECT_EQ(p.at(Port::P3), 3);
	EXPECT_EQ(p.at(Port::P4), 4);
	EXPECT_THROW(p.at(Port::NONE), std::out_of_range);
}

TEST(PortArrayTest, TestIndex) {
	PortArray<int> p = {1, 2, 3, 4};

	EXPECT_EQ(p[Port::P1], 1);
	EXPECT_EQ(p[Port::P2], 2);
	EXPECT_EQ(p[Port::P3], 3);
	EXPECT_EQ(p[Port::P4], 4);
}

TEST(PortArrayTest, TestFront) {
	PortArray<int> p = {1, 2, 3, 4};

	EXPECT_EQ(p.front(), 1);
}

TEST(PortArrayTest, TestBack) {
	PortArray<int> p = {1, 2, 3, 4};

	EXPECT_EQ(p.back(), 4);
}

TEST(PortArrayTest, TestData) {
	PortArray<int> p = {1, 2, 3, 4};

	EXPECT_EQ(p.data(), &p[Port::P1]);
}

TEST(PortArrayTest, TestBegin) {
	PortArray<int> p = {1, 2, 3, 4};

	EXPECT_EQ(*p.begin(), p[Port::P1]);
	EXPECT_EQ(*p.cbegin(), p[Port::P1]);
}

TEST(PortArrayTest, TestEnd) {
	PortArray<int> p = {1, 2, 3, 4};

	EXPECT_EQ(p.end(), p.begin() + 4);
	EXPECT_EQ(p.cend(), p.cbegin() + 4);
}

TEST(PortArrayTest, TestRBegin) {
	PortArray<int> p = {1, 2, 3, 4};

	EXPECT_EQ(*p.rbegin(), p[Port::P4]);
	EXPECT_EQ(*p.crbegin(), p[Port::P4]);
}

TEST(PortArrayTest, TestREnd) {
	PortArray<int> p = {1, 2, 3, 4};

	EXPECT_EQ(p.rend(), p.rbegin() + 4);
	EXPECT_EQ(p.crend(), p.crbegin() + 4);
}

TEST(PortArrayTest, TestEmpty) {
	PortArray<int> p = {1, 2, 3, 4};

	EXPECT_FALSE(p.empty());
}

TEST(PortArrayTest, TestSize) {
	PortArray<int> p = {1, 2, 3, 4};

	EXPECT_EQ(p.size(), 4);
}

TEST(PortArrayTest, TestMaxSize) {
	PortArray<int> p = {1, 2, 3, 4};

	EXPECT_EQ(p.max_size(), 4);
}

TEST(PortArrayTest, TestFill) {
	PortArray<int> p = {1, 2, 3, 4};
	p.fill(5);

	EXPECT_EQ(p, (PortArray<int>{5, 5, 5, 5}));
}

TEST(PortArrayTest, TestSwap) {
	PortArray<int> p1 = {1, 2, 3, 4};
	PortArray<int> p2 = {5, 6, 7, 8};
	p1.swap(p2);

	EXPECT_EQ(p1, (PortArray<int>{5, 6, 7, 8}));
	EXPECT_EQ(p2, (PortArray<int>{1, 2, 3, 4}));
}

}  // namespace cpp_slippi::utils
