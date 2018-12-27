#include "pch.h"
#include "PieceTestsHead.h"

class RockTests : public PieceTests {};
TEST_F(RockTests, validMoves) {
	const Rock rock(Position("C4"));
	for (const auto& column : Position::getAllColumns())
		EXPECT_TRUE(rock.isConsistentWithMoveRules(Position(column+"4")));

	for (const auto& row : Position::getAllRows()) 
		EXPECT_TRUE(rock.isConsistentWithMoveRules(Position("C" + row)));
}
TEST_F(RockTests, invalidMoves) {
	const Rock rock(Position("C4"));
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
TEST_F(RockTests, moveUsesMoveRules) {
	Rock rock(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(rock.isConsistentWithMoveRules(newPosition)).Return(true);
	rock.move(newPosition);
	ASSERT_WAS_CALLED(rock.isConsistentWithMoveRules(newPosition));
}
TEST_F(RockTests, attackUsesMoveRules) {
	Rock rock(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(rock.isConsistentWithAttackRules(newPosition)).Return(true);
	rock.attack(newPosition);
	ASSERT_WAS_CALLED(rock.isConsistentWithAttackRules(newPosition));
}
TEST_F(RockTests, attackRulesUsesMoveRules) {
	const Rock rock(Position("C4"));
	const Position newPosition("B3");

	WHEN_CALLED(rock.isConsistentWithMoveRules(newPosition)).Return(true);
	rock.isConsistentWithAttackRules(newPosition);
	ASSERT_WAS_CALLED(rock.isConsistentWithMoveRules(newPosition));
}