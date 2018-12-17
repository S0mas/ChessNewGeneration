#include "pch.h"
#include "PieceTestsHead.h"

class PawnTests : public PieceTests {};
TEST(PawnTests, validMovesWasNotMoved) {
	Pawn pawn(Position("C2"));
	EXPECT_TRUE(pawn.isConsistentWithMoveRules(Position("C3")));
	EXPECT_TRUE(pawn.isConsistentWithMoveRules(Position("C4")));
}

TEST(PawnTests, validMovesWasMoved) {
	Pawn pawn(Position("C1"));
	pawn.move(Position("C2"));
	EXPECT_TRUE(pawn.isConsistentWithMoveRules(Position("C3")));
}
TEST(PawnTests, invalidMoves) {
	Pawn pawn(Position("C2"));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("D2")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("D3")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("B4")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("B2")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("B3")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("B4")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("C1")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("C5")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("B1")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("D1")));
}
TEST(PawnTests, invalidMovesWasMoved) {
	Pawn pawn(Position("C1"));
	pawn.move(Position("C2"));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("C4")));
}
TEST(PawnTests, moveUsesMoveRulesTests) {
	const auto pawn = std::make_unique<Pawn>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(pawn->isConsistentWithMoveRules(newPosition)).Return(true);
	pawn->move(newPosition);
	ASSERT_WAS_CALLED(pawn->isConsistentWithMoveRules(newPosition));
}
TEST(PawnTests, attackUsesMoveRulesTests) {
	const auto pawn = std::make_unique<Pawn>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(pawn->isConsistentWithAttackRules(newPosition)).Return(true);
	pawn->attack(newPosition);
	ASSERT_WAS_CALLED(pawn->isConsistentWithAttackRules(newPosition));
}
TEST(PawnTests, checkingCollisions) {
	const auto pawn = std::make_unique<Pawn>(Position("C4"));

	EXPECT_TRUE(pawn->isCheckingCollisions());
}