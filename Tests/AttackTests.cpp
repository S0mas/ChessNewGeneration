#pragma once
#include "pch.h"
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/Pieces.h"
#include "../ChessNewGeneration/Rules.h"
#include <memory>

class AttackTests : public ::testing::Test {
public:
	void TearDown() override {
		ISOLATOR_CLEANUP();
	}
};
struct AttackTestingObjects {
	ChessBoard cbMock;
	Rock attacker{ Rock(Position("F8")) };
	Bishop targetWhite{ Bishop(Position("A1")) };
	Bishop targetBlack{ Bishop(Position("A1"), Player::Black) };
	King targetKing{ King(Position("A1"), Player::Black) };
};

TEST_F(AttackTests, legalAttack) {
	const AttackTestingObjects objects;
	PRIVATE_WHEN_CALLED(_, Rules::isAttackPossible,
		TYPEOF(const Piece&), TYPEOF(const std::unique_ptr<Piece>&), TYPEOF(const ChessBoard&)).Return(true);

	EXPECT_TRUE(Rules::isAttackLegal(objects.attacker, objects.targetBlack, objects.cbMock));
}

TEST_F(AttackTests, notLegalAttack_cantAttackKing) {
	const AttackTestingObjects objects;
	PRIVATE_WHEN_CALLED(_, Rules::isAttackPossible,
		TYPEOF(const Piece&), TYPEOF(const std::unique_ptr<Piece>&), TYPEOF(const ChessBoard&)).Return(true);

	EXPECT_FALSE(Rules::isAttackLegal(objects.attacker, objects.targetKing, objects.cbMock));
}

TEST_F(AttackTests, notLegalAttack_impossible) {
	const AttackTestingObjects objects;
	PRIVATE_WHEN_CALLED(_, Rules::isAttackPossible,
		TYPEOF(const Piece&), TYPEOF(const std::unique_ptr<Piece>&), TYPEOF(const ChessBoard&)).Return(false);

	EXPECT_FALSE(Rules::isAttackLegal(objects.attacker, objects.targetBlack, objects.cbMock));
}

TEST_F(AttackTests, possibleAttack) {
	const AttackTestingObjects objects;

	WHEN_CALLED(objects.cbMock.isThereCollision(ANY_REF(std::vector<Position>))).Return(false);
	WHEN_CALLED(objects.attacker.isConsistentWithAttackRules(ANY_REF(Position))).Return(true);

	EXPECT_TRUE(Rules::isAttackPossible(objects.attacker, objects.targetBlack, objects.cbMock));
}

TEST_F(AttackTests, impossibleAttack_invalidPath) {
	const AttackTestingObjects objects;
	WHEN_CALLED(objects.cbMock.isThereCollision(ANY_REF(std::vector<Position>))).Return(false);
	WHEN_CALLED(objects.attacker.isConsistentWithAttackRules(ANY_REF(Position))).Return(false);

	EXPECT_FALSE(Rules::isAttackPossible(objects.attacker, objects.targetBlack, objects.cbMock));
}

TEST_F(AttackTests, impossibleAttack_collision) {
	const AttackTestingObjects objects;
	WHEN_CALLED(objects.cbMock.isThereCollision(ANY_REF(std::vector<Position>))).Return(true);
	WHEN_CALLED(objects.attacker.isConsistentWithAttackRules(ANY_REF(Position))).Return(true);

	EXPECT_FALSE(Rules::isAttackPossible(objects.attacker, objects.targetBlack, objects.cbMock));
}

TEST_F(AttackTests, impossibleAttack_sameOwner) {
	const AttackTestingObjects objects;
	WHEN_CALLED(objects.cbMock.isThereCollision(ANY_REF(std::vector<Position>))).Return(false);
	WHEN_CALLED(objects.attacker.isConsistentWithAttackRules(ANY_REF(Position))).Return(true);

	EXPECT_FALSE(Rules::isAttackPossible(objects.attacker, objects.targetWhite, objects.cbMock));
}