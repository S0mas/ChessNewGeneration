#include "pch.h"
#include <gtest/gtest.h>

#include "../ChessNewGeneration/Pieces.h"
TEST(KingTests, validMoves) {
	King king(Position("C4"));

	Position newPosition("C3");
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == newPosition);

	newPosition = "B3";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == newPosition);

	newPosition = "B4";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == newPosition);

	newPosition = "C5";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == newPosition);

	newPosition = "D5";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == newPosition);

	newPosition = "C6";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == newPosition);

	newPosition = "B5";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == newPosition);

	newPosition = "A6";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == newPosition);
}

TEST(KingTests, invalidMoves) {
	King king(Position("C4"));

	const auto oldPosition = king.getPosition();
	Position newPosition("A4");
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == oldPosition);

	newPosition = "C6";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == oldPosition);

	newPosition = "B2";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == oldPosition);

	newPosition = "B6";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == oldPosition);

	newPosition = "A3";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == oldPosition);

	newPosition = "A5";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == oldPosition);

	newPosition = "E4";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == oldPosition);

	newPosition = "C2";
	king.move(newPosition);
	EXPECT_TRUE(king.getPosition() == oldPosition);
}