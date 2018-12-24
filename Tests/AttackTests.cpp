#pragma once
#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/Pieces.h"
#include "../ChessNewGeneration/Rules.h"
#include <memory>

class AttackTests : public ::testing::Test {
public:
	void TearDown() final {
		ISOLATOR_CLEANUP();
	}
};

TEST(AttackTests, legalAttack) {
	ChessBoard cbMock;
	PRIVATE_WHEN_CALLED(_, Rules::isAttackPossible,
		TYPEOF(const Piece&), TYPEOF(const std::unique_ptr<Piece>&), TYPEOF(const ChessBoard&)).Return(true);

	std::unique_ptr<Piece> rock = std::make_unique<Rock>(Position("F8"));
	Bishop bishop(Position("A1"), Player::Black);
	EXPECT_TRUE(Rules::isAttackLegal(bishop, rock, cbMock));
}

TEST(AttackTests, notLegalAttack_cantAttackKing) {
	ChessBoard cbMock;
	PRIVATE_WHEN_CALLED(_, Rules::isAttackPossible,
		TYPEOF(const Piece&), TYPEOF(const std::unique_ptr<Piece>&), TYPEOF(const ChessBoard&)).Return(true);

	Bishop bishop(Position("A1"), Player::Black);
	std::unique_ptr<Piece> king = std::make_unique<King>(Position("F8"));

	EXPECT_FALSE(Rules::isAttackLegal(bishop, king, cbMock));
}

TEST(AttackTests, notLegalAttack_impossible) {
	ChessBoard cbMock;
	PRIVATE_WHEN_CALLED(_, Rules::isAttackPossible,
		TYPEOF(const Piece&), TYPEOF(const std::unique_ptr<Piece>&), TYPEOF(const ChessBoard&)).Return(false);

	Bishop bishop(Position("A1"), Player::Black);
	std::unique_ptr<Piece> king = std::make_unique<King>(Position("F8"));

	EXPECT_FALSE(Rules::isAttackLegal(bishop, king, cbMock));
}

TEST(AttackTests, possibleAttack) {
	ChessBoard cbMock;
	std::unique_ptr<Piece> rock = std::make_unique<Rock>(Position("D3"));
	std::unique_ptr<Piece> knigth = std::make_unique<Pawn>(Position("E6"), Player::Black);

	PRIVATE_WHEN_CALLED(_, Rules::isThereCollisions,
		TYPEOF(const std::vector<Position>&), TYPEOF(const ChessBoard&)).Return(false);
	WHEN_CALLED(rock->isConsistentWithAttackRules(ANY_REF(Position))).Return(true);

	std::cout << Rules::isAttackPossible(*rock, knigth, cbMock);
}

TEST(AttackTests, impossibleAttack_KingAsTarget) {
	ChessBoard cbMock;

	std::unique_ptr<Piece> rock = std::make_unique<Rock>(Position("D2"), Player::Black);
	std::unique_ptr<Piece> king = std::make_unique<King>(Position("F8"));

	PRIVATE_WHEN_CALLED(_, Rules::isThereCollisions,
		TYPEOF(const std::vector<Position>&), TYPEOF(const ChessBoard&)).Return(false);
	WHEN_CALLED(rock->isConsistentWithAttackRules(ANY_REF(Position))).Return(false);

	EXPECT_FALSE(Rules::isAttackPossible(*rock, king, cbMock));
}

TEST(AttackTests, impossibleAttack_invalidPath) {
	ChessBoard cbMock;

	std::unique_ptr<Piece> rock = std::make_unique<Rock>(Position("D2"), Player::Black);
	std::unique_ptr<Piece> pawn = std::make_unique<Pawn>(Position("F8"));

	PRIVATE_WHEN_CALLED(_, Rules::isThereCollisions,
		TYPEOF(const std::vector<Position>&), TYPEOF(const ChessBoard&)).Return(false);
	WHEN_CALLED(rock->isConsistentWithAttackRules(ANY_REF(Position))).Return(false);

	EXPECT_FALSE(Rules::isAttackPossible(*rock, pawn, cbMock));
}

TEST(AttackTests, impossibleAttack_collision) {
	ChessBoard cbMock;
	Rock rock(Position("D2"), Player::Black);
	std::unique_ptr<Piece> pawn = std::make_unique<Pawn>(Position("F8"));

	PRIVATE_WHEN_CALLED(_, Rules::isThereCollisions,
		TYPEOF(const std::vector<Position>&), TYPEOF(const ChessBoard&)).Return(true);
	WHEN_CALLED(rock.isConsistentWithAttackRules(ANY_REF(Position))).Return(true);

	EXPECT_FALSE(Rules::isAttackPossible(rock, pawn, cbMock));
}

TEST(AttackTests, impossibleAttack_sameOwner) {
	ChessBoard cbMock;
	Rock rock(Position("D2"));
	std::unique_ptr<Piece> pawn = std::make_unique<Pawn>(Position("F8"));

	PRIVATE_WHEN_CALLED(_, Rules::isThereCollisions,
		TYPEOF(const std::vector<Position>&), TYPEOF(const ChessBoard&)).Return(true);
	WHEN_CALLED(rock.isConsistentWithAttackRules(ANY_REF(Position))).Return(true);

	EXPECT_FALSE(Rules::isAttackPossible(rock, pawn, cbMock));
}