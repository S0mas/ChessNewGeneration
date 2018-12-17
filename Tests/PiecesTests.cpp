#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/Pieces.h"

class KingTests : public ::testing::Test {
public:
	void TearDown() final {
		ISOLATOR_CLEANUP();
	}
};

TEST(KingTests, validMoves) {
	King king(Position("C4"));

	Position newPosition("C3");
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "B3";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "B4";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "C5";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "D5";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "C6";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "B5";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "A6";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);
}

TEST(KingTests, invalidMoves) {
	King king(Position("C4"));

	const auto oldPosition = king.getPosition();
	Position newPosition("A4");
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "C6";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "B2";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "B6";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "A3";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "A5";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "E4";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "C2";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);
}

TEST(KingTests, moveUsesMoveRulesTests) {
	auto king = std::make_unique<King>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(king->isConsistentWithMoveRules(newPosition)).Return(true);
	king->move(newPosition);
	ASSERT_WAS_CALLED(king->isConsistentWithMoveRules(newPosition));
}

TEST(KingTests, attackUsesMoveRulesTests) {
	auto king = std::make_unique<King>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(king->isConsistentWithAttackRules(newPosition)).Return(true);
	king->attack(newPosition);
	ASSERT_WAS_CALLED(king->isConsistentWithAttackRules(newPosition));
}

TEST(KingTests, attackRulesUsesMoveRules) {
	auto king = std::make_unique<King>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(king->isConsistentWithMoveRules(newPosition)).Return(true);
	auto p = king->isConsistentWithAttackRules(newPosition);
	ASSERT_WAS_CALLED(king->isConsistentWithMoveRules(newPosition));
}
