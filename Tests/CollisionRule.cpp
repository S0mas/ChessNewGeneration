#pragma once
#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/Pieces.h"
#include "../ChessNewGeneration/Rules.h"

class CollisionTests : public ::testing::Test {
public:
	void TearDown() final {
		ISOLATOR_CLEANUP();
	}
};

TEST(CollisionTests, noCollision) {
	CollisionRule colisionRule;

	std::vector<Piece*> pieces;
	Bishop bishop(Position("D4"));
	pieces.emplace_back(new Bishop(Position("D3")));
	pieces.emplace_back(new Pawn(Position("E4")));
	pieces.emplace_back(new Knight(Position("F5")));
	pieces.emplace_back(new King(Position("E6")));
	
	EXPECT_TRUE(colisionRule.eval(bishop.getRoute(Position("F6")), pieces));
}

TEST(CollisionTests, collision) {
	CollisionRule colisionRule;

	std::vector<Piece*> pieces;
	Bishop bishop(Position("D4"));
	pieces.emplace_back(new Bishop(Position("D3")));
	pieces.emplace_back(new Pawn(Position("E5")));
	pieces.emplace_back(new Knight(Position("F5")));
	pieces.emplace_back(new King(Position("E6")));

	EXPECT_FALSE(colisionRule.eval(bishop.getRoute(Position("F6")), pieces));
}