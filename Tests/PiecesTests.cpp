#include "pch.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../ChessNewGeneration/Pieces.h"

using ::testing::AtLeast;

class MockKing : public King {
public:
	MockKing(const Position& pos) : King(pos) {}

	MOCK_CONST_METHOD1(isConsistentWithMoveRules, bool(const Position&));
	MOCK_CONST_METHOD1(isConsistentWithAttackRules, bool(const Position&));
	MOCK_METHOD1(move, void(const Position&) noexcept);
	MOCK_METHOD1(attack, void(const Position&) noexcept);

};

TEST(KingTests, validMoves) {
	King king(Position("C4"));

	Position newPosition("C3");
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "B3";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "B4";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "C5";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "D5";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "C6";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "B5";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);

	newPosition = "A6";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);
}

TEST(KingTests, invalidMoves) {
	King king(Position("C4"));

	const auto oldPosition = king.getPosition();
	Position newPosition("A4");
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "C6";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "B2";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "B6";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "A3";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "A5";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "E4";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);

	newPosition = "C2";
	king.move(newPosition);
	EXPECT_EQ(king.getPosition(), oldPosition);
}

TEST(KingTests, attackTests) {
	MockKing king(Position("C4"));
	Position newPosition("A3");

	EXPECT_CALL(king, isConsistentWithMoveRules(newPosition)).Times(1);
	king.attack(newPosition);
	EXPECT_EQ(king.getPosition(), newPosition);
}
