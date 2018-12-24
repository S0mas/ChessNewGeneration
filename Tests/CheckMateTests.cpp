#pragma once
#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/Chessboard.h"
#include "../ChessNewGeneration/Rules.h"

class CheckMateTests : public ::testing::Test {
public:
	void TearDown() final {
		ISOLATOR_CLEANUP();
	}
};

TEST(CheckMateTests, whiteIsMated) {
	ChessBoard cbMock;
	std::vector<Position> moves;
	moves.push_back(Position("A1"));
	WHEN_CALLED(Rules::isLegalMove).Return(false);
	PRIVATE_WHEN_CALLED(_, Rules::getAllValidMovesForPiece, TYPEOF(const Piece&), TYPEOF(const ChessBoard&)).Return(BY_VAL(moves));

	EXPECT_TRUE(Rules::isThereCheckMate(Player::White, cbMock));
}

TEST(CheckMateTests, whiteIsNotMated) {
	ChessBoard cbMock;
	std::vector<Position> moves;
	moves.push_back(Position("A1"));
	WHEN_CALLED(Rules::isLegalMove).Return(true);
	PRIVATE_WHEN_CALLED(_, Rules::getAllValidMovesForPiece, TYPEOF(const Piece&), TYPEOF(const ChessBoard&)).Return(BY_VAL(moves));

	EXPECT_TRUE(Rules::isThereCheckMate(Player::White, cbMock));
}