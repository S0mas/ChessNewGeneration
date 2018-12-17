#include "pch.h"
#include "PieceTestsHead.h"

class BishopTests : public PieceTests {};
TEST(BishopTests, validMoves) {
	Bishop bishop(Position("C4"));
	EXPECT_TRUE(bishop.isConsistentWithMoveRules(Position("A2")));
	EXPECT_TRUE(bishop.isConsistentWithMoveRules(Position("B3")));
	EXPECT_TRUE(bishop.isConsistentWithMoveRules(Position("D5")));
	EXPECT_TRUE(bishop.isConsistentWithMoveRules(Position("E6")));
	EXPECT_TRUE(bishop.isConsistentWithMoveRules(Position("F7")));
	EXPECT_TRUE(bishop.isConsistentWithMoveRules(Position("G8")));
	EXPECT_TRUE(bishop.isConsistentWithMoveRules(Position("A6")));
	EXPECT_TRUE(bishop.isConsistentWithMoveRules(Position("B5")));
	EXPECT_TRUE(bishop.isConsistentWithMoveRules(Position("D3")));
	EXPECT_TRUE(bishop.isConsistentWithMoveRules(Position("E2")));
	EXPECT_TRUE(bishop.isConsistentWithMoveRules(Position("F1")));
}
TEST(BishopTests, invalidMoves) {
	Bishop bishop(Position("C4"));
	EXPECT_FALSE(bishop.isConsistentWithMoveRules(Position("D6")));
	EXPECT_FALSE(bishop.isConsistentWithMoveRules(Position("D2")));
	EXPECT_FALSE(bishop.isConsistentWithMoveRules(Position("B6")));
	EXPECT_FALSE(bishop.isConsistentWithMoveRules(Position("B2")));
	EXPECT_FALSE(bishop.isConsistentWithMoveRules(Position("C5")));
	EXPECT_FALSE(bishop.isConsistentWithMoveRules(Position("C6")));
	EXPECT_FALSE(bishop.isConsistentWithMoveRules(Position("B4")));
	EXPECT_FALSE(bishop.isConsistentWithMoveRules(Position("D4")));
	EXPECT_FALSE(bishop.isConsistentWithMoveRules(Position("G4")));
	EXPECT_FALSE(bishop.isConsistentWithMoveRules(Position("C8")));
}
TEST(BishopTests, moveUsesMoveRulesTests) {
	const auto bishop = std::make_unique<Bishop>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(bishop->isConsistentWithMoveRules(newPosition)).Return(true);
	bishop->move(newPosition);
	ASSERT_WAS_CALLED(bishop->isConsistentWithMoveRules(newPosition));
}
TEST(BishopTests, attackUsesMoveRulesTests) {
	const auto bishop = std::make_unique<Bishop>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(bishop->isConsistentWithAttackRules(newPosition)).Return(true);
	bishop->attack(newPosition);
	ASSERT_WAS_CALLED(bishop->isConsistentWithAttackRules(newPosition));
}
TEST(BishopTests, attackRulesUsesMoveRules) {
	const auto bishop = std::make_unique<Bishop>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(bishop->isConsistentWithMoveRules(newPosition)).Return(true);
	auto p = bishop->isConsistentWithAttackRules(newPosition);
	ASSERT_WAS_CALLED(bishop->isConsistentWithMoveRules(newPosition));
}
TEST(BishopTests, checkingCollisions) {
	const auto bishop = std::make_unique<Bishop>(Position("C4"));

	EXPECT_TRUE(bishop->isCheckingCollisions());
}
