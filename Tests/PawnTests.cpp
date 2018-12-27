#include "pch.h"
#include "PieceTestsHead.h"

class PawnTests : public PieceTests {};
TEST_F(PawnTests, validMovesAfterFirstMove) {
	Pawn pawn(Position("C2"));
	EXPECT_TRUE(pawn.isConsistentWithMoveRules(Position("C3")));
	EXPECT_TRUE(pawn.isConsistentWithMoveRules(Position("C4")));
}

TEST_F(PawnTests, validMovesBeforeFirstMove) {
	Pawn pawn(Position("C1"));
	pawn.move(Position("C2"));
	EXPECT_TRUE(pawn.isConsistentWithMoveRules(Position("C3")));
}
TEST_F(PawnTests, invalidMoves) {
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
TEST_F(PawnTests, invalidMovesAfterFirstMove) {
	Pawn pawn(Position("C1"), Player::Black);
	pawn.move(Position("C2"));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("C4")));
}

TEST_F(PawnTests, validMovesWasBeforeFirstMove_Black) {
	Pawn pawn(Position("C7"), Player::Black);
	EXPECT_TRUE(pawn.isConsistentWithMoveRules(Position("C6")));
	EXPECT_TRUE(pawn.isConsistentWithMoveRules(Position("C5")));
}

TEST_F(PawnTests, validMovesAfterFirstMove_Black) {
	Pawn pawn(Position("C8"), Player::Black);
	pawn.move(Position("C7"));
	EXPECT_TRUE(pawn.isConsistentWithMoveRules(Position("C6")));
}
TEST_F(PawnTests, invalidMoves_Black) {
	Pawn pawn(Position("C6"), Player::Black);
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("D2")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("D3")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("C7")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("B2")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("B3")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("B4")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("C1")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("C8")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("B1")));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("D1")));
}
TEST_F(PawnTests, invalidMovesAfterFirstMove_Black) {
	Pawn pawn(Position("C1"), Player::Black);
	pawn.move(Position("C2"));
	EXPECT_FALSE(pawn.isConsistentWithMoveRules(Position("C4")));
}

TEST_F(PawnTests, moveUsesMoveRules) {
	Pawn pawn(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(pawn.isConsistentWithMoveRules(newPosition)).Return(true);
	pawn.move(newPosition);
	ASSERT_WAS_CALLED(pawn.isConsistentWithMoveRules(newPosition));
}
TEST_F(PawnTests, attackUsesAttackRules) {
	Pawn pawn(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(pawn.isConsistentWithAttackRules(newPosition)).Return(true);
	pawn.attack(newPosition);
	ASSERT_WAS_CALLED(pawn.isConsistentWithAttackRules(newPosition));
}