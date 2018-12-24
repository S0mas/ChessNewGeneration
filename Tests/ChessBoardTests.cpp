#pragma once
#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/Chessboard.h"

class ChessBoardTests : public ::testing::Test {
public:
	void TearDown() final {
		ISOLATOR_CLEANUP();
	}
};

TEST(ChessBoardTests, positionOccupied) {
	ChessBoard cb;
	EXPECT_TRUE(cb.isPositionOccupied(Position("D1")));
}

TEST(ChessBoardTests, positionOccupied2) {
	ChessBoard cb;
	EXPECT_FALSE(cb.isPositionOccupied(Position("D4")));
}