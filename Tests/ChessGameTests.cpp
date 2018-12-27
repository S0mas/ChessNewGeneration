#pragma once
#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/ChessGame.h"

class ChessGameTests : public ::testing::Test {
public:
	void TearDown() override {
		ISOLATOR_CLEANUP();
	}
};

struct AttackTestingObjects {
	ChessBoard cbMock;
	Rook attacker{ Rook(Position("F8")) };
	Bishop targetWhite{ Bishop(Position("A1")) };
	Bishop targetBlack{ Bishop(Position("A1"), Player::Black) };
	King targetKing{ King(Position("A1"), Player::Black) };
};

TEST_F(ChessGameTests, checkmate) {
	ChessGame chess;
	PRIVATE_WHEN_CALLED(&chess, checkIfMoveIsLegal, TYPEOF(const SimpleMove&)).Return(false);
	EXPECT_TRUE(chess.isThereCheckmate());
}

TEST_F(ChessGameTests, noCheckmate) {
	ChessGame chess;
	std::vector<Position> moves;
	moves.emplace_back(Position("A1"));
	PRIVATE_WHEN_CALLED(&chess, getAllValidMovesForPiece, TYPEOF(const Piece&)).Return(BY_VAL(moves));
	PRIVATE_WHEN_CALLED(&chess, checkIfMoveIsLegal, TYPEOF(const SimpleMove&)).Return(true);
	EXPECT_FALSE(chess.isThereCheckmate());
}

TEST_F(ChessGameTests, check) {
	ChessGame chess;
	PRIVATE_WHEN_CALLED(&chess, isAttackPossible, TYPEOF(const Piece&), TYPEOF(const Piece&)).Return(true);
	EXPECT_TRUE(chess.isThereCheck(Player::White));
}

TEST_F(ChessGameTests, noCheck) {
	ChessGame chess;
	PRIVATE_WHEN_CALLED(&chess, isAttackPossible, TYPEOF(const Piece&), TYPEOF(const Piece&)).Return(false);
	EXPECT_FALSE(chess.isThereCheck(Player::Black));
}

TEST_F(ChessGameTests, ownerIsActivePlayer) {
	ChessGame chess;
	const King king(Position("A1"));
	EXPECT_TRUE(chess.isOwnerActivePlayer(king));
}

TEST_F(ChessGameTests, ownerIsNotActivePlayer) {
	ChessGame chess;
	const King king(Position("A1"), Player::Black);
	EXPECT_FALSE(chess.isOwnerActivePlayer(king));
}

TEST_F(ChessGameTests, attackIsPossible) {
	ChessGame chess;
	const Bishop attacker(Position("A1"));
	const Bishop target(Position("A6"), Player::Black);
	WHEN_CALLED(attacker.isConsistentWithAttackRules(ANY_REF(Position))).Return(true);
	WHEN_CALLED(chess.chessboard_.isThereCollision(ANY_REF(std::vector<Position>))).Return(false);
	EXPECT_TRUE(chess.isAttackPossible(attacker, target));
}

TEST_F(ChessGameTests, attackIsImpossible_sameOwner) {
	ChessGame chess;
	const Bishop attacker(Position("A1"));
	const Bishop target(Position("A6"));
	WHEN_CALLED(attacker.isConsistentWithAttackRules(ANY_REF(Position))).Return(true);
	WHEN_CALLED(chess.chessboard_.isThereCollision(ANY_REF(std::vector<Position>))).Return(false);
	EXPECT_FALSE(chess.isAttackPossible(attacker, target));
}

TEST_F(ChessGameTests, attackIsImpossible_collision) {
	ChessGame chess;
	const Bishop attacker(Position("A1"));
	const Bishop target(Position("A6"), Player::Black);
	WHEN_CALLED(attacker.isConsistentWithAttackRules(ANY_REF(Position))).Return(true);
	WHEN_CALLED(chess.chessboard_.isThereCollision(ANY_REF(std::vector<Position>))).Return(true);
	EXPECT_FALSE(chess.isAttackPossible(attacker, target));
}

TEST_F(ChessGameTests, attackIsImpossible_invalidDestinatino) {
	ChessGame chess;
	const Bishop attacker(Position("A1"));
	const Bishop target(Position("A6"), Player::Black);
	WHEN_CALLED(attacker.isConsistentWithAttackRules(ANY_REF(Position))).Return(false);
	WHEN_CALLED(chess.chessboard_.isThereCollision(ANY_REF(std::vector<Position>))).Return(false);
	EXPECT_FALSE(chess.isAttackPossible(attacker, target));
}

TEST_F(ChessGameTests, moveIsPossible) {
	ChessGame chess;
	const Bishop mover(Position("A1"));
	const Position destination("A6");
	WHEN_CALLED(mover.isConsistentWithMoveRules(ANY_REF(Position))).Return(true);
	WHEN_CALLED(chess.chessboard_.isThereCollision(ANY_REF(std::vector<Position>))).Return(false);
	WHEN_CALLED(chess.chessboard_.isPositionOccupied(ANY_REF(Position))).Return(false);
	EXPECT_TRUE(chess.isMovePossible(mover, destination));
}

TEST_F(ChessGameTests, moveIsImpossible_destinationOcupied) {
	ChessGame chess;
	const Bishop mover(Position("A1"));
	const Position destination("A6");
	WHEN_CALLED(mover.isConsistentWithMoveRules(ANY_REF(Position))).Return(true);
	WHEN_CALLED(chess.chessboard_.isThereCollision(ANY_REF(std::vector<Position>))).Return(false);
	WHEN_CALLED(chess.chessboard_.isPositionOccupied(ANY_REF(Position))).Return(true);
	EXPECT_FALSE(chess.isMovePossible(mover, destination));
}

TEST_F(ChessGameTests, moveIsImpossible_collision) {
	ChessGame chess;
	const Bishop mover(Position("A1"));
	const Position destination("A6");
	WHEN_CALLED(mover.isConsistentWithMoveRules(ANY_REF(Position))).Return(true);
	WHEN_CALLED(chess.chessboard_.isThereCollision(ANY_REF(std::vector<Position>))).Return(true);
	WHEN_CALLED(chess.chessboard_.isPositionOccupied(ANY_REF(Position))).Return(false);
	EXPECT_FALSE(chess.isMovePossible(mover, destination));
}

TEST_F(ChessGameTests, moveIsImpossible_invalidDestinatino) {
	ChessGame chess;
	const Bishop mover(Position("A1"));
	const Position destination("A6");
	WHEN_CALLED(mover.isConsistentWithMoveRules(ANY_REF(Position))).Return(false);
	WHEN_CALLED(chess.chessboard_.isThereCollision(ANY_REF(std::vector<Position>))).Return(false);
	WHEN_CALLED(chess.chessboard_.isPositionOccupied(ANY_REF(Position))).Return(false);
	EXPECT_FALSE(chess.isMovePossible(mover, destination));
}

TEST_F(ChessGameTests, attackIsLegal) {
	ChessGame chess;
	const Bishop attacker(Position("A1"));
	const Bishop target(Position("A6"), Player::Black);

	WHEN_CALLED(target.isKing()).Return(false);
	WHEN_CALLED(chess.isAttackPossible(ANY_REF(Piece), ANY_REF(Piece))).Return(true);
	WHEN_CALLED(chess.isConsistentWithOtherRules(ANY_REF(SimpleMove))).Return(true);
	EXPECT_TRUE(chess.isAttackLegal(attacker, target));
}

TEST_F(ChessGameTests, attackIsIllegal_kingIsTarget) {
	ChessGame chess;
	const Bishop attacker(Position("A1"));
	const Bishop target(Position("A6"), Player::Black);

	WHEN_CALLED(target.isKing()).Return(true);
	WHEN_CALLED(chess.isAttackPossible(ANY_REF(Piece), ANY_REF(Piece))).Return(true);
	WHEN_CALLED(chess.isConsistentWithOtherRules(ANY_REF(SimpleMove))).Return(true);
	EXPECT_FALSE(chess.isAttackLegal(attacker, target));
}

TEST_F(ChessGameTests, attackIsIllegal_impossibleAttack) {
	ChessGame chess;
	const Bishop attacker(Position("A1"));
	const Bishop target(Position("A6"), Player::Black);

	WHEN_CALLED(target.isKing()).Return(true);
	WHEN_CALLED(chess.isAttackPossible(ANY_REF(Piece), ANY_REF(Piece))).Return(true);
	WHEN_CALLED(chess.isConsistentWithOtherRules(ANY_REF(SimpleMove))).Return(true);
	EXPECT_FALSE(chess.isAttackLegal(attacker, target));
}

TEST_F(ChessGameTests, attackIsIllegal_isNotConsistentWithOtherRules) {
	ChessGame chess;
	const Bishop attacker(Position("A1"));
	const Bishop target(Position("A6"), Player::Black);

	WHEN_CALLED(target.isKing()).Return(true);
	WHEN_CALLED(chess.isAttackPossible(ANY_REF(Piece), ANY_REF(Piece))).Return(true);
	WHEN_CALLED(chess.isConsistentWithOtherRules(ANY_REF(SimpleMove))).Return(true);
	EXPECT_FALSE(chess.isAttackLegal(attacker, target));
}

TEST_F(ChessGameTests, moveIsLegal) {
	ChessGame chess;
	const Bishop mover(Position("A1"));
	const Position destination("A6");

	WHEN_CALLED(chess.isMovePossible(ANY_REF(Piece), ANY_REF(Position))).Return(true);
	WHEN_CALLED(chess.isConsistentWithOtherRules(ANY_REF(SimpleMove))).Return(true);
	EXPECT_TRUE(chess.isMoveLegal(mover, destination));
}

TEST_F(ChessGameTests, moveIsIllegal_impossibleMove) {
	ChessGame chess;
	const Bishop mover(Position("A1"));
	const Position destination("A6");

	WHEN_CALLED(chess.isMovePossible(ANY_REF(Piece), ANY_REF(Position))).Return(false);
	WHEN_CALLED(chess.isConsistentWithOtherRules(ANY_REF(SimpleMove))).Return(true);
	EXPECT_FALSE(chess.isMoveLegal(mover, destination));
}

TEST_F(ChessGameTests, moveIsIllegal_isNotConsistentWithOtherRules) {
	ChessGame chess;
	const Bishop mover(Position("A1"));
	const Position destination("A6");

	WHEN_CALLED(chess.isMovePossible(ANY_REF(Piece), ANY_REF(Position))).Return(true);
	WHEN_CALLED(chess.isConsistentWithOtherRules(ANY_REF(SimpleMove))).Return(false);
	EXPECT_FALSE(chess.isMoveLegal(mover, destination));
}