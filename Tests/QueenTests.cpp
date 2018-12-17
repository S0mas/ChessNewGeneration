#include "pch.h"
#include "PieceTestsHead.h"

class QueenTests : public PieceTests {};
TEST(QueenTests, validMoves) {
	Queen queen(Position("C4"));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("A4")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("B4")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("D4")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("E4")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("F4")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("G4")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("H4")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("C1")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("C2")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("C3")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("C5")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("C6")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("C7")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("C8")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("A2")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("B3")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("D5")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("E6")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("F7")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("G8")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("A6")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("B5")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("D3")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("E2")));
	EXPECT_TRUE(queen.isConsistentWithMoveRules(Position("F1")));
}
TEST(QueenTests, invalidMoves) {
	Queen queen(Position("C4"));
	EXPECT_FALSE(queen.isConsistentWithMoveRules(Position("A1")));
	EXPECT_FALSE(queen.isConsistentWithMoveRules(Position("B1")));
	EXPECT_FALSE(queen.isConsistentWithMoveRules(Position("B2")));
	EXPECT_FALSE(queen.isConsistentWithMoveRules(Position("A3")));
	EXPECT_FALSE(queen.isConsistentWithMoveRules(Position("A5")));
	EXPECT_FALSE(queen.isConsistentWithMoveRules(Position("D1")));
	EXPECT_FALSE(queen.isConsistentWithMoveRules(Position("D7")));
	EXPECT_FALSE(queen.isConsistentWithMoveRules(Position("F2")));
	EXPECT_FALSE(queen.isConsistentWithMoveRules(Position("D6")));
}
TEST(QueenTests, moveUsesMoveRulesTests) {
	const auto queen = std::make_unique<Queen>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(queen->isConsistentWithMoveRules(newPosition)).Return(true);
	queen->move(newPosition);
	ASSERT_WAS_CALLED(queen->isConsistentWithMoveRules(newPosition));
}
TEST(QueenTests, attackUsesMoveRulesTests) {
	const auto queen = std::make_unique<Queen>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(queen->isConsistentWithAttackRules(newPosition)).Return(true);
	queen->attack(newPosition);
	ASSERT_WAS_CALLED(queen->isConsistentWithAttackRules(newPosition));
}
TEST(QueenTests, attackRulesUsesMoveRules) {
	const auto queen = std::make_unique<Queen>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(queen->isConsistentWithMoveRules(newPosition)).Return(true);
	auto p = queen->isConsistentWithAttackRules(newPosition);
	ASSERT_WAS_CALLED(queen->isConsistentWithMoveRules(newPosition));
}
TEST(QueenTests, checkingCollisions) {
	const auto queen = std::make_unique<Queen>(Position("C4"));

	EXPECT_TRUE(queen->isCheckingCollisions());
}