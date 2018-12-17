#include "pch.h"
#include "PieceTestsHead.h"

class RockTests : public PieceTests {};
TEST(RockTests, validMoves) {
	Rock rock(Position("C4"));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("A4")));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("B4")));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("D4")));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("E4")));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("F4")));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("G4")));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("H4")));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("C1")));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("C2")));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("C3")));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("C5")));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("C6")));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("C7")));
	EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("C8")));
}
TEST(RockTests, invalidMoves) {
	Rock rock(Position("C4"));
	EXPECT_FALSE(rock.isConsistentWithMoveRules(Position("D7")));
	EXPECT_FALSE(rock.isConsistentWithMoveRules(Position("D5")));
	EXPECT_FALSE(rock.isConsistentWithMoveRules(Position("D3")));
	EXPECT_FALSE(rock.isConsistentWithMoveRules(Position("B5")));
	EXPECT_FALSE(rock.isConsistentWithMoveRules(Position("B3")));
	EXPECT_FALSE(rock.isConsistentWithMoveRules(Position("A8")));
	EXPECT_FALSE(rock.isConsistentWithMoveRules(Position("A1")));
	EXPECT_FALSE(rock.isConsistentWithMoveRules(Position("D1")));
	EXPECT_FALSE(rock.isConsistentWithMoveRules(Position("H3")));
	EXPECT_FALSE(rock.isConsistentWithMoveRules(Position("G5")));
}
TEST(RockTests, moveUsesMoveRulesTests) {
	const auto rock = std::make_unique<Rock>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(rock->isConsistentWithMoveRules(newPosition)).Return(true);
	rock->move(newPosition);
	ASSERT_WAS_CALLED(rock->isConsistentWithMoveRules(newPosition));
}
TEST(RockTests, attackUsesMoveRulesTests) {
	const auto rock = std::make_unique<Rock>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(rock->isConsistentWithAttackRules(newPosition)).Return(true);
	rock->attack(newPosition);
	ASSERT_WAS_CALLED(rock->isConsistentWithAttackRules(newPosition));
}
TEST(RockTests, attackRulesUsesMoveRules) {
	const auto rock = std::make_unique<Rock>(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(rock->isConsistentWithMoveRules(newPosition)).Return(true);
	auto p = rock->isConsistentWithAttackRules(newPosition);
	ASSERT_WAS_CALLED(rock->isConsistentWithMoveRules(newPosition));
}
TEST(RockTests, checkingCollisions) {
	const auto rock = std::make_unique<Rock>(Position("C4"));

	EXPECT_TRUE(rock->isCheckingCollisions());
}
