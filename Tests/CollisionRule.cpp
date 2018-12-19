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
	std::vector<const Piece*> pieces;
	Bishop bishop(Position("D4"));
	pieces.emplace_back(new Bishop(Position("D3")));
	pieces.emplace_back(new Pawn(Position("E4")));
	pieces.emplace_back(new Knight(Position("F5")));
	pieces.emplace_back(new King(Position("E6")));
	
	EXPECT_TRUE(CollisionRule::isThereNoCollisions(bishop.getRoute(Position("F6")), pieces));
}

TEST(CollisionTests, collisionDiag) {
	std::vector<const Piece*> pieces;
	Bishop bishop(Position("D4"));
	pieces.emplace_back(new Bishop(Position("D3")));
	pieces.emplace_back(new Pawn(Position("E5")));
	pieces.emplace_back(new Knight(Position("F5")));
	pieces.emplace_back(new King(Position("E6")));

	EXPECT_FALSE(CollisionRule::isThereNoCollisions(bishop.getRoute(Position("F6")), pieces));
}

TEST(CollisionTests, collisionColumn) {
	std::vector<const Piece*> pieces;
	Rock rock(Position("D4"));
	pieces.emplace_back(new Bishop(Position("D3")));
	pieces.emplace_back(new Pawn(Position("E5")));
	pieces.emplace_back(new Knight(Position("F5")));
	pieces.emplace_back(new King(Position("D7")));

	EXPECT_FALSE(CollisionRule::isThereNoCollisions(rock.getRoute(Position("D8")), pieces));
}

TEST(CollisionTests, collisionRow) {
	std::vector<const Piece*> pieces;
	Queen queen(Position("D4"));
	pieces.emplace_back(new Bishop(Position("D3")));
	pieces.emplace_back(new Pawn(Position("E5")));
	pieces.emplace_back(new Knight(Position("F4")));
	pieces.emplace_back(new King(Position("D7")));

	EXPECT_FALSE(CollisionRule::isThereNoCollisions(queen.getRoute(Position("G4")), pieces));
}