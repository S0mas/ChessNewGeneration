#pragma once
#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#define Tests
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
	PRIVATE_WHEN_CALLED(&chess, isThereCheck, TYPEOF(const Player&)).Return(true);
	EXPECT_TRUE(chess.isThereCheckmate());
	EXPECT_FALSE(chess.isThereStalemate());
}

TEST_F(ChessGameTests, noCheckmate_stalemate) {
	ChessGame chess;
	PRIVATE_WHEN_CALLED(&chess, checkIfMoveIsLegal, TYPEOF(const SimpleMove&)).Return(false);
	PRIVATE_WHEN_CALLED(&chess, isThereCheck, TYPEOF(const Player&)).Return(false);
	EXPECT_FALSE(chess.isThereCheckmate());
	EXPECT_TRUE(chess.isThereStalemate());
}

TEST_F(ChessGameTests, noCheckmate_haveLegalMovesNoCheck) {
	ChessGame chess;
	PRIVATE_WHEN_CALLED(&chess, checkIfMoveIsLegal, TYPEOF(const SimpleMove&)).Return(true);
	PRIVATE_WHEN_CALLED(&chess, isThereCheck, TYPEOF(const Player&)).Return(false);
	EXPECT_FALSE(chess.isThereCheckmate());
	EXPECT_FALSE(chess.isThereStalemate());
}

TEST_F(ChessGameTests, noCheckmate_haveLegalMoves) {
	ChessGame chess;
	PRIVATE_WHEN_CALLED(&chess, checkIfMoveIsLegal, TYPEOF(const SimpleMove&)).Return(true);
	PRIVATE_WHEN_CALLED(&chess, isThereCheck, TYPEOF(const Player&)).Return(true);
	EXPECT_FALSE(chess.isThereCheckmate());
	EXPECT_FALSE(chess.isThereStalemate());
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
	chess.activePlayer_ = Player::White;
	EXPECT_TRUE(chess.isOwnerActivePlayer(king));
}

TEST_F(ChessGameTests, ownerIsNotActivePlayer) {
	ChessGame chess;
	const King king(Position("A1"), Player::Black);
	chess.activePlayer_ = Player::White;
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

	WHEN_CALLED(target.isKing()).Return(false);
	WHEN_CALLED(chess.isAttackPossible(ANY_REF(Piece), ANY_REF(Piece))).Return(false);
	WHEN_CALLED(chess.isConsistentWithOtherRules(ANY_REF(SimpleMove))).Return(true);
	EXPECT_FALSE(chess.isAttackLegal(attacker, target));
}

TEST_F(ChessGameTests, attackIsIllegal_isNotConsistentWithOtherRules) {
	ChessGame chess;
	const Bishop attacker(Position("A1"));
	const Bishop target(Position("A6"), Player::Black);

	WHEN_CALLED(target.isKing()).Return(false);
	WHEN_CALLED(chess.isAttackPossible(ANY_REF(Piece), ANY_REF(Piece))).Return(true);
	WHEN_CALLED(chess.isConsistentWithOtherRules(ANY_REF(SimpleMove))).Return(false);
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

TEST_F(ChessGameTests, consistentWithOtherRules) {
	ChessGame chess;
	WHEN_CALLED(chess.isThereCheck(ANY_REF(Player))).Return(false);
	EXPECT_TRUE(chess.isConsistentWithOtherRules({ Position("A1"), Position("A5") }));
}

TEST_F(ChessGameTests, notConsistentWithOtherRules) {
	ChessGame chess;
	WHEN_CALLED(chess.isThereCheck(ANY_REF(Player))).Return(true);
	EXPECT_FALSE(chess.isConsistentWithOtherRules({ Position("A1"), Position("A5") }));
}

TEST_F(ChessGameTests, moveIsLegal_move) {
	ChessGame chess;
	WHEN_CALLED(chess.isOwnerActivePlayer(ANY_REF(Piece))).Return(true);
	WHEN_CALLED(chess.isMoveLegal(ANY_REF(Piece), ANY_REF(Position))).Return(true);
	WHEN_CALLED(chess.isAttackLegal(ANY_REF(Piece), ANY_REF(Piece))).Return(false);
	EXPECT_TRUE(chess.checkIfMoveIsLegal({ Position("A1"), Position("A5") }));
}

TEST_F(ChessGameTests, moveIsLegal_attack) {
	ChessGame chess;
	WHEN_CALLED(chess.isOwnerActivePlayer(ANY_REF(Piece))).Return(true);
	WHEN_CALLED(chess.isMoveLegal(ANY_REF(Piece), ANY_REF(Position))).Return(false);
	WHEN_CALLED(chess.isAttackLegal(ANY_REF(Piece), ANY_REF(Piece))).Return(true);
	EXPECT_TRUE(chess.checkIfMoveIsLegal({ Position("A1"), Position("A7") }));
}

TEST_F(ChessGameTests, moveIsIlegal_pieceNotFound) {
	ChessGame chess;
	WHEN_CALLED(chess.isOwnerActivePlayer(ANY_REF(Piece))).Return(true);
	WHEN_CALLED(chess.isMoveLegal(ANY_REF(Piece), ANY_REF(Position))).Return(true);
	WHEN_CALLED(chess.isAttackLegal(ANY_REF(Piece), ANY_REF(Piece))).Return(true);
	EXPECT_FALSE(chess.checkIfMoveIsLegal({ Position("A3"), Position("A5") }));
}

TEST_F(ChessGameTests, moveIsIlegal_ownerIsNotActivePlayer) {
	ChessGame chess;
	WHEN_CALLED(chess.isOwnerActivePlayer(ANY_REF(Piece))).Return(false);
	WHEN_CALLED(chess.isMoveLegal(ANY_REF(Piece), ANY_REF(Position))).Return(true);
	WHEN_CALLED(chess.isAttackLegal(ANY_REF(Piece), ANY_REF(Piece))).Return(true);
	EXPECT_FALSE(chess.checkIfMoveIsLegal({ Position("A1"), Position("A5") }));
}

TEST_F(ChessGameTests, moveIsIlegal_breaksMoveRules) {
	ChessGame chess;
	WHEN_CALLED(chess.isOwnerActivePlayer(ANY_REF(Piece))).Return(true);
	WHEN_CALLED(chess.isMoveLegal(ANY_REF(Piece), ANY_REF(Position))).Return(false);
	WHEN_CALLED(chess.isAttackLegal(ANY_REF(Piece), ANY_REF(Piece))).Return(true);
	EXPECT_FALSE(chess.checkIfMoveIsLegal({ Position("A1"), Position("A5") }));
}

TEST_F(ChessGameTests, moveIsIlegal_breaksAttackRules) {
	ChessGame chess;
	WHEN_CALLED(chess.isOwnerActivePlayer(ANY_REF(Piece))).Return(true);
	WHEN_CALLED(chess.isMoveLegal(ANY_REF(Piece), ANY_REF(Position))).Return(true);
	WHEN_CALLED(chess.isAttackLegal(ANY_REF(Piece), ANY_REF(Piece))).Return(false);
	EXPECT_FALSE(chess.checkIfMoveIsLegal({ Position("A1"), Position("A7") }));
}

TEST_F(ChessGameTests, gameEnded) {
	ChessGame chess;
	WHEN_CALLED(chess.isThereCheckmate()).Return(true);
	EXPECT_TRUE(chess.isGameEnded());
}