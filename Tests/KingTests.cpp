#include "pch.h"
#include "PieceTestsHead.h"

class KingTests : public PieceTests {};

TEST(KingTests, validMoves) {
	King king(Position("C4"));

	EXPECT_TRUE(king.isConsistentWithMoveRules(Position("C3")));
	EXPECT_TRUE(king.isConsistentWithMoveRules(Position("C5")));
	EXPECT_TRUE(king.isConsistentWithMoveRules(Position("B4")));
	EXPECT_TRUE(king.isConsistentWithMoveRules(Position("D4")));
	EXPECT_TRUE(king.isConsistentWithMoveRules(Position("B3")));
	EXPECT_TRUE(king.isConsistentWithMoveRules(Position("B5")));
	EXPECT_TRUE(king.isConsistentWithMoveRules(Position("D3")));
	EXPECT_TRUE(king.isConsistentWithMoveRules(Position("D5")));
}

TEST(KingTests, invalidMoves) {
	King king(Position("C4"));

	EXPECT_FALSE(king.isConsistentWithMoveRules(Position("A1")));
	EXPECT_FALSE(king.isConsistentWithMoveRules(Position("A2")));
	EXPECT_FALSE(king.isConsistentWithMoveRules(Position("A3")));
	EXPECT_FALSE(king.isConsistentWithMoveRules(Position("B2")));
	EXPECT_FALSE(king.isConsistentWithMoveRules(Position("B6")));
	EXPECT_FALSE(king.isConsistentWithMoveRules(Position("C6")));
	EXPECT_FALSE(king.isConsistentWithMoveRules(Position("C2")));
	EXPECT_FALSE(king.isConsistentWithMoveRules(Position("D6")));
}

TEST(KingTests, moveUsesMoveRulesTests) {
	const auto king = std::make_unique<King>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(king->isConsistentWithMoveRules(newPosition)).Return(true);
	king->move(newPosition);
	ASSERT_WAS_CALLED(king->isConsistentWithMoveRules(newPosition));
}

TEST(KingTests, attackUsesMoveRulesTests) {
	const auto king = std::make_unique<King>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(king->isConsistentWithAttackRules(newPosition)).Return(true);
	king->attack(newPosition);
	ASSERT_WAS_CALLED(king->isConsistentWithAttackRules(newPosition));
}

TEST(KingTests, attackRulesUsesMoveRules) {
	const auto king = std::make_unique<King>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(king->isConsistentWithMoveRules(newPosition)).Return(true);
	auto p = king->isConsistentWithAttackRules(newPosition);
	ASSERT_WAS_CALLED(king->isConsistentWithMoveRules(newPosition));
}

TEST(KingTests, checkingCollisions) {
	const auto king = std::make_unique<King>(Position("C4"));

	EXPECT_TRUE(king->isCheckingCollisions());
}