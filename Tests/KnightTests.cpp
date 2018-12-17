#include "pch.h"
#include "PieceTestsHead.h"

class KnightTests : public PieceTests {};
TEST(KnightTests, validMoves) {
	Knight knight(Position("C4"));
	EXPECT_TRUE(knight.isConsistentWithMoveRules(Position("B6")));
	EXPECT_TRUE(knight.isConsistentWithMoveRules(Position("D6")));
	EXPECT_TRUE(knight.isConsistentWithMoveRules(Position("E5")));
	EXPECT_TRUE(knight.isConsistentWithMoveRules(Position("E3")));
	EXPECT_TRUE(knight.isConsistentWithMoveRules(Position("D2")));
	EXPECT_TRUE(knight.isConsistentWithMoveRules(Position("B2")));
	EXPECT_TRUE(knight.isConsistentWithMoveRules(Position("A3")));
	EXPECT_TRUE(knight.isConsistentWithMoveRules(Position("A5")));
}
TEST(KnightTests, invalidMoves) {
	Knight knight(Position("C4"));
	EXPECT_FALSE(knight.isConsistentWithMoveRules(Position("C7")));
	EXPECT_FALSE(knight.isConsistentWithMoveRules(Position("D5")));
	EXPECT_FALSE(knight.isConsistentWithMoveRules(Position("B3")));
	EXPECT_FALSE(knight.isConsistentWithMoveRules(Position("B1")));
	EXPECT_FALSE(knight.isConsistentWithMoveRules(Position("C5")));
	EXPECT_FALSE(knight.isConsistentWithMoveRules(Position("C6")));
	EXPECT_FALSE(knight.isConsistentWithMoveRules(Position("B4")));
	EXPECT_FALSE(knight.isConsistentWithMoveRules(Position("D4")));
	EXPECT_FALSE(knight.isConsistentWithMoveRules(Position("G4")));
	EXPECT_FALSE(knight.isConsistentWithMoveRules(Position("H8")));
}
TEST(KnightTests, moveUsesMoveRulesTests) {
	const auto knight = std::make_unique<Knight>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(knight->isConsistentWithMoveRules(newPosition)).Return(true);
	knight->move(newPosition);
	ASSERT_WAS_CALLED(knight->isConsistentWithMoveRules(newPosition));
}
TEST(KnightTests, attackUsesMoveRulesTests) {
	const auto knight = std::make_unique<Knight>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(knight->isConsistentWithAttackRules(newPosition)).Return(true);
	knight->attack(newPosition);
	ASSERT_WAS_CALLED(knight->isConsistentWithAttackRules(newPosition));
}
TEST(KnightTests, attackRulesUsesMoveRules) {
	const auto knight = std::make_unique<Knight>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(knight->isConsistentWithMoveRules(newPosition)).Return(true);
	auto p = knight->isConsistentWithAttackRules(newPosition);
	ASSERT_WAS_CALLED(knight->isConsistentWithMoveRules(newPosition));
}
TEST(KnightTests, checkingCollisions) {
	const auto knight = std::make_unique<Knight>(Position("C4"));

	EXPECT_FALSE(knight->isCheckingCollisions());
}