#pragma once
#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/Pieces.h"
#include "../ChessNewGeneration/Rules.h"

class CollisionTests : public ::testing::Test {
public:
	void TearDown() final {
		ISOLATOR_CLEANUP();
	}
};

TEST(CollisionTests, noCollision) {
	ChessBoard cbMock;
	std::vector<Position> dummyRoute;
	dummyRoute.push_back(Position("D4"));
	dummyRoute.push_back(Position("D5"));

	EXPECT_FALSE(Rules::isThereCollisions(dummyRoute, cbMock));
}

TEST(CollisionTests, collision) {
	ChessBoard cbMock;
	std::vector<Position> dummyRoute;
	dummyRoute.push_back(Position("D1"));
	dummyRoute.push_back(Position("D2"));

	EXPECT_TRUE(Rules::isThereCollisions(dummyRoute, cbMock));
}