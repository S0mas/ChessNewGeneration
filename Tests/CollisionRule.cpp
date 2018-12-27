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

TEST_F(CollisionTests, noCollision) {
	ChessBoard cbMock;
	std::vector<Position> dummyRoute;
	dummyRoute.push_back(Position("D4"));
	WHEN_CALLED(cbMock.isPositionOccupied(ANY_REF(Position))).Return(false);

	EXPECT_FALSE(cbMock.isThereCollision(dummyRoute));
}

TEST_F(CollisionTests, collision) {
	ChessBoard cbMock;
	std::vector<Position> dummyRoute;
	dummyRoute.push_back(Position("D4"));
	WHEN_CALLED(cbMock.isPositionOccupied(ANY_REF(Position))).Return(true);

	EXPECT_TRUE(cbMock.isThereCollision(dummyRoute));
}