#include "pch.h"
#include "PieceTestsHead.h"

class RookTests : public PieceTests {};
TEST_F(RookTests, validMoves) {
	const Rook rook(Position("C4"));
	for (const auto& column : Position::getAllColumns())
		EXPECT_TRUE(rook.isConsistentWithMoveRules(Position(column+"4")));

	for (const auto& row : Position::getAllRows()) 
		EXPECT_TRUE(rook.isConsistentWithMoveRules(Position("C" + row)));
}
TEST_F(RookTests, invalidMoves) {
	const Rook rook(Position("C4"));
	for (const auto& column : Position::getAllColumns())
		for (const auto& row : Position::getAllRows())
			if(column != "C" && row != "4")
				EXPECT_FALSE(rook.isConsistentWithMoveRules(Position(column + row)));
}
TEST_F(RookTests, moveUsesMoveRules) {
	Rook rook(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(rook.isConsistentWithMoveRules(newPosition)).Return(true);
	rook.move(newPosition);
	ASSERT_WAS_CALLED(rook.isConsistentWithMoveRules(newPosition));
}
TEST_F(RookTests, attackUsesMoveRules) {
	Rook rook(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(rook.isConsistentWithAttackRules(newPosition)).Return(true);
	rook.attack(newPosition);
	ASSERT_WAS_CALLED(rook.isConsistentWithAttackRules(newPosition));
}
TEST_F(RookTests, attackRulesUsesMoveRules) {
	const Rook rook(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(rook.isConsistentWithMoveRules(newPosition)).Return(true);
	rook.isConsistentWithAttackRules(newPosition);
	ASSERT_WAS_CALLED(rook.isConsistentWithMoveRules(newPosition));
}