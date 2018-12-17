#pragma once
#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/Pieces.h"
#include "../ChessNewGeneration/Rules.h"

class AttackTests : public ::testing::Test {
public:
	void TearDown() final {
		ISOLATOR_CLEANUP();
	}
};

TEST(AttackTests, validAttacks) {
	AttackRule attackRule;
	King king(Position("D4"), Player::White);
	Queen queen(Position("C5"), Player::Black);

	EXPECT_TRUE(attackRule.eval(king, queen));

	Pawn pawn(Position("D2"), Player::White);
	Knight knight(Position("C3"), Player::Black);

	EXPECT_TRUE(attackRule.eval(pawn, knight));

	Pawn pawn2(Position("D4"), Player::Black);
	Bishop bishop(Position("E3"), Player::White);

	EXPECT_TRUE(attackRule.eval(pawn2, bishop));
}

TEST(AttackTests, attackingKingNoAllowed) {
	AttackRule attackRule;
	Pawn pawn(Position("D2"), Player::White);
	King king(Position("C3"), Player::Black);

	EXPECT_FALSE(attackRule.eval(pawn, king));
}

TEST(AttackTests, attackingSameOwnerNotAllowed) {
	AttackRule attackRule;
	King king(Position("D4"), Player::White);
	Queen queen(Position("C5"), Player::White);

	EXPECT_FALSE(attackRule.eval(king, queen));
}