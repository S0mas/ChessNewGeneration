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

TEST_F(CheckMateTests, whiteIsMated) {
	ChessBoard cbMock;
	WHEN_CALLED(Rules::isLegalMove, TYPEOF(const Player&), TYPEOF(const SimpleMove&), TYPEOF(ChessBoard&)).Return(false);

	EXPECT_TRUE(Rules::isThereCheckMate(Player::White, cbMock));
}

TEST_F(CheckMateTests, whiteIsNotMated) {
	ChessBoard cbMock;
	WHEN_CALLED(Rules::isLegalMove, TYPEOF(const Player&), TYPEOF(const SimpleMove&), TYPEOF(ChessBoard&)).Return(true);

	EXPECT_TRUE(Rules::isThereCheckMate(Player::White, cbMock));
}