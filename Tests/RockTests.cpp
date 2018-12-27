#include "pch.h"
#include "PieceTestsHead.h"

class RockTests : public PieceTests {};
TEST_F(RockTests, validMoves) {
	const Rook rook(Position("C4"));
	for (const auto& column : Position::getAllColumns())
		EXPECT_TRUE(rook.isConsistentWithMoveRules(Position(column+"4")));

	for (const auto& row : Position::getAllRows()) 
		EXPECT_TRUE(rook.isConsistentWithMoveRules(Position("C" + row)));
}
TEST_F(RockTests, invalidMoves) {
	const Rook rook(Position("C4"));
	EXPECT_FALSE(rook.isConsistentWithMoveRules(Position("D7")));
	EXPECT_FALSE(rook.isConsistentWithMoveRules(Position("D5")));
	EXPECT_FALSE(rook.isConsistentWithMoveRules(Position("D3")));
	EXPECT_FALSE(rook.isConsistentWithMoveRules(Position("B5")));
	EXPECT_FALSE(rook.isConsistentWithMoveRules(Position("B3")));
	EXPECT_FALSE(rook.isConsistentWithMoveRules(Position("A8")));
	EXPECT_FALSE(rook.isConsistentWithMoveRules(Position("A1")));
	EXPECT_FALSE(rook.isConsistentWithMoveRules(Position("D1")));
	EXPECT_FALSE(rook.isConsistentWithMoveRules(Position("H3")));
	EXPECT_FALSE(rook.isConsistentWithMoveRules(Position("G5")));
}
TEST_F(RockTests, moveUsesMoveRules) {
	Rook rook(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(rook.isConsistentWithMoveRules(newPosition)).Return(true);
	rook.move(newPosition);
	ASSERT_WAS_CALLED(rook.isConsistentWithMoveRules(newPosition));
}
TEST_F(RockTests, attackUsesMoveRules) {
	Rook rook(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(rook.isConsistentWithAttackRules(newPosition)).Return(true);
	rook.attack(newPosition);
	ASSERT_WAS_CALLED(rook.isConsistentWithAttackRules(newPosition));
}
TEST_F(RockTests, attackRulesUsesMoveRules) {
	const Rook rook(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(rook.isConsistentWithMoveRules(newPosition)).Return(true);
	rook.isConsistentWithAttackRules(newPosition);
	ASSERT_WAS_CALLED(rook.isConsistentWithMoveRules(newPosition));
}