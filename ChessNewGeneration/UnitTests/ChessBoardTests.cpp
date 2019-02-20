#pragma once
#include "pch.h"
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include "../Chessboard.h"

class ChessBoardTests : public ::testing::Test {
public:
	void TearDown() final {
	}
};

TEST_F(ChessBoardTests, isOccupied) {
	ChessBoard cb;
	EXPECT_TRUE(cb.isPositionOccupied(Position("D1")));
}

TEST_F(ChessBoardTests,isNotOccupied) {
	ChessBoard cb;
	EXPECT_FALSE(cb.isPositionOccupied(Position("D4")));
}

TEST_F(ChessBoardTests, findKing) {
	ChessBoard cb;
	const auto& king = cb.findKing(Player::White);
	EXPECT_EQ((*king)->getPosition(), Position("E1"));
	EXPECT_EQ((*king)->getOwner(), Player::White);
}

TEST_F(ChessBoardTests, findKing_black) {
	ChessBoard cb;
	const auto& king = cb.findKing(Player::Black);
	EXPECT_EQ((*king)->getPosition(), Position("E8"));
	EXPECT_EQ((*king)->getOwner(), Player::Black);
}

TEST_F(ChessBoardTests, getPieceByPosition) {
	ChessBoard cb;
	auto king = cb.getPieceByPosition(Position("E1"));
	EXPECT_TRUE((*king)->isKing());
	EXPECT_EQ((*king)->getPosition(), Position("E1"));
	EXPECT_EQ((*king)->getOwner(), Player::White);
}

TEST_F(ChessBoardTests, notFound) {
	ChessBoard cb;
	const auto& king = cb.getPieceByPosition(Position("E3"));
	EXPECT_EQ(king, cb.notFound());
}

TEST_F(ChessBoardTests, found) {
	ChessBoard cb;
	const auto& king = cb.getPieceByPosition(Position("E1"));
	EXPECT_TRUE(king != cb.notFound());
}

TEST_F(ChessBoardTests, doMove) {
	ChessBoard cb;

	EXPECT_EQ(cb.getPieceByPosition(Position("E4")), cb.notFound());
	EXPECT_TRUE(cb.getPieceByPosition(Position("E2")) != cb.notFound());

	cb.doMove({ Position("E2"), Position("E4") });

	EXPECT_EQ(cb.getPieceByPosition(Position("E2")), cb.notFound());
	EXPECT_TRUE(cb.getPieceByPosition(Position("E4")) != cb.notFound());
}

TEST_F(ChessBoardTests, undoMove) {
	ChessBoard cb;

	EXPECT_EQ(cb.getPieceByPosition(Position("E4")), cb.notFound());
	EXPECT_TRUE(cb.getPieceByPosition(Position("E2")) != cb.notFound());

	cb.doMove({ Position("E2"), Position("E4") });
	cb.undoMove();

	EXPECT_EQ(cb.getPieceByPosition(Position("E4")), cb.notFound());
	EXPECT_TRUE(cb.getPieceByPosition(Position("E2")) != cb.notFound());
}

TEST_F(ChessBoardTests, getPieces) {
	ChessBoard cb;
	const auto& begin = cb.getPieces().begin();
	const auto& end = cb.getPieces().end();
	EXPECT_EQ(std::distance(begin, end), 32);
}