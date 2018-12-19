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
	King king(Position("D4"), Player::White);
	Queen queen(Position("C5"), Player::Black);

	EXPECT_TRUE(AttackRule::isAttackValidMove(king, queen));

	Pawn pawn(Position("D2"), Player::White);
	Knight knight(Position("C3"), Player::Black);

	EXPECT_TRUE(AttackRule::isAttackValidMove(pawn, knight));

	Pawn pawn2(Position("D4"), Player::Black);
	Bishop bishop(Position("E3"), Player::White);

	EXPECT_TRUE(AttackRule::isAttackValidMove(pawn2, bishop));
}

TEST(AttackTests, attackingKingNoAllowed) {
	Pawn pawn(Position("D2"), Player::White);
	King king(Position("C3"), Player::Black);

	EXPECT_FALSE(AttackRule::isAttackValidMove(pawn, king));
}

TEST(AttackTests, attackingSameOwnerNotAllowed) {
	King king(Position("D4"), Player::White);
	Queen queen(Position("C5"), Player::White);

	EXPECT_FALSE(AttackRule::isAttackValidMove(king, queen));
}