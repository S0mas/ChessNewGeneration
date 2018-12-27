#pragma once
#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/Pieces.h"
#include "../ChessNewGeneration/Rules.h"

class CheckTests : public ::testing::Test {
public:
	void TearDown() final {
		ISOLATOR_CLEANUP();
	}
};

/*TEST_F(CheckTests, noCheck) {
	ChessBoard cbMock;
	King king(Position("A1"));
	PRIVATE_WHEN_CALLED(_, Rules::isAttackPossible,
		TYPEOF(const Piece&), TYPEOF(const std::unique_ptr<Piece>&), TYPEOF(const ChessBoard&)).Return(false);
	WHEN_CALLED(cbMock.findKing(ANY_REF(Player))).Return(&king);
	EXPECT_FALSE(Rules::isThereCheck(Player::White, cbMock));
}*/

/*TEST_F(CheckTests, check) {
	ChessBoard cbMock;
	PRIVATE_WHEN_CALLED(_, Rules::isAttackPossible,
		TYPEOF(const Piece&), TYPEOF(const std::unique_ptr<Piece>&), TYPEOF(const ChessBoard&)).Return(true);

	EXPECT_TRUE(Rules::isThereCheck(Player::White, cbMock));
}*/
