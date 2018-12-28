#pragma once
#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/ChessPieces.h"

class ChessPiecesTests : public ::testing::Test {
public:
	void TearDown() final {
		ISOLATOR_CLEANUP();
	}
};

TEST_F(ChessPiecesTests, size) {
	ChessPieces cp;
	const auto size = std::distance(cp.begin(), cp.end());

	EXPECT_EQ(size, 32);
}

TEST_F(ChessPiecesTests, killPiece_invalidValueEnd) {
	ChessPieces cp;
	auto size = std::distance(cp.begin(), cp.end());
	EXPECT_EQ(size, 32);
	cp.killPiece(32);
	size = std::distance(cp.begin(), cp.end());
	EXPECT_EQ(size, 32);
}

TEST_F(ChessPiecesTests, killPiece_validValueBegin) {
	ChessPieces cp;
	auto size = std::distance(cp.begin(), cp.end());
	EXPECT_EQ(size, 32);
	cp.killPiece(31);
	size = std::distance(cp.begin(), cp.end());
	EXPECT_EQ(size, 31);
}

TEST_F(ChessPiecesTests, resurrectPiece) {
	ChessPieces cp;
	cp.killPiece(31);
	cp.resurrectLastKilledPiece();
	const auto size = std::distance(cp.begin(), cp.end());
	EXPECT_EQ(size, 32);
}