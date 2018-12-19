#pragma once
#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/Pieces.h"
#include "../ChessNewGeneration/Rules.h"

class CheckTests : public ::testing::Test {
public:
	void TearDown() final {
		ISOLATOR_CLEANUP();
	}
};

TEST(CheckTests, noWhiteCheck_BlackCheck) {
	std::vector<const Piece*> pieces;
	
	pieces.emplace_back(new King(Position("A1")));
	pieces.emplace_back(new Pawn(Position("A2")));
	pieces.emplace_back(new Rock(Position("A8"), Player::Black));
	pieces.emplace_back(new King(Position("B3"), Player::Black));

	EXPECT_FALSE(CheckRule::isThereNoCheck(Player::Black, pieces));
	EXPECT_TRUE(CheckRule::isThereNoCheck(Player::White, pieces));
}