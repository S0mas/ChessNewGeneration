#pragma once
#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/Pieces.h"
#include "../ChessNewGeneration/Rules.h"

class CheckMateTests : public ::testing::Test {
public:
	void TearDown() final {
		ISOLATOR_CLEANUP();
	}
};

TEST(CheckMateTests, whiteIsMated) {
	std::vector<const Piece*> pieces;

	pieces.emplace_back(new King(Position("A1")));
	pieces.emplace_back(new Pawn(Position("A2")));
	pieces.emplace_back(new Rock(Position("A8"), Player::Black));
	pieces.emplace_back(new Rock(Position("B7"), Player::Black));
	pieces.emplace_back(new Bishop(Position("C3"), Player::Black));
	pieces.emplace_back(new King(Position("C5"), Player::Black));

	EXPECT_TRUE(CheckMateRule::isThereCheckMate(Player::White, pieces));
}

TEST(CheckMateTests, whiteIsNotMated) {
	std::vector<const Piece*> pieces;

	pieces.emplace_back(new King(Position("A1")));
	pieces.emplace_back(new Pawn(Position("A2")));
	pieces.emplace_back(new Rock(Position("A8"), Player::Black));
	pieces.emplace_back(new Rock(Position("A7"), Player::Black));
	pieces.emplace_back(new Bishop(Position("C3"), Player::Black));
	pieces.emplace_back(new King(Position("B5"), Player::Black));

	EXPECT_FALSE(CheckMateRule::isThereCheckMate(Player::White, pieces));
}