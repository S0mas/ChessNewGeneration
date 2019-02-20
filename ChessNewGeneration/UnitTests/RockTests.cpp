#include "pch.h"
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include "PieceTestsHead.h"
#include "../Chessboard.h"

class RookTests : public PieceTests {};
TEST_F(RookTests, validMoves) {
	const Rook rook(Position("C4"));
	for (const auto& column : Board::getAllColumns())
		EXPECT_TRUE(rook.isConsistentWithMoveRules(Position(std::string{ column, '4' })));

	for (const auto& row : Board::getAllRows())
		EXPECT_TRUE(rook.isConsistentWithMoveRules(Position(std::string{ 'C', row })));
}
TEST_F(RookTests, invalidMoves) {
	const Rook rook(Position("C4"));
	for (const auto& column : Board::getAllColumns())
		for (const auto& row : Board::getAllRows())
			if(column != 'C' && row != '4')
				EXPECT_FALSE(rook.isConsistentWithMoveRules(Position(std::string({ column, row }))));
}
TEST_F(RookTests, attackRulesUsesMoveRules) {
	const Rook rook(Position("C4"));
	const Position newPosition("B3");

	//WHEN_CALLED(rook.isConsistentWithMoveRules(newPosition)).Return(true);
	//rook.isConsistentWithAttackRules(newPosition);
	//ASSERT_WAS_CALLED(rook.isConsistentWithMoveRules(newPosition));
}