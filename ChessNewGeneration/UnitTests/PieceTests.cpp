#include "pch.h"
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include "PieceTestsHead.h"

class PieceMock final : public Piece {
public:
	PieceMock(const Position& pos = Position("A1")) : Piece(pos) {}
	bool isConsistentWithMoveRules(const Position& newPosition) const noexcept final {
		return newPosition == Position("B4");
	}

	bool isConsistentWithAttackRules(const Position& newPosition) const noexcept final {
		return newPosition == Position("B4");
	}
};

TEST_F(PieceTests, isKing) {
	const King king(Position("A1"));

	EXPECT_TRUE(king.isKing());
}

TEST_F(PieceTests, isNotKing) {
	const Bishop bishop(Position("A1"));

	EXPECT_FALSE(bishop.isKing());
}

TEST_F(PieceTests, getPosition) {
	const Bishop bishop(Position("A3"));

	EXPECT_EQ(bishop.getPosition(), Position("A3"));
}

TEST_F(PieceTests, getOwner) {
	const Bishop bishopB(Position("A3"), Player::Black);
	const Bishop bishopW(Position("A3"), Player::White);
	EXPECT_EQ(bishopB.getOwner(), Player::Black);
	EXPECT_EQ(bishopW.getOwner(), Player::White);
}

TEST_F(PieceTests, setPosition) {
	Bishop bishop(Position("A3"));
	EXPECT_TRUE(bishop.hasNotMoved());
	bishop.setPosition(Position("A6"));
	EXPECT_FALSE(bishop.hasNotMoved());
	EXPECT_EQ(bishop.getPosition(), Position("A6"));
}

TEST_F(PieceTests, getRoute_ValidMove_InvalidAttack) {
	Queen queen(Position("D5"));
	std::vector<Position> route;
	route.push_back(Position("E4"));
	EXPECT_EQ(route, queen.getRoute(Position("F3")));
}

TEST_F(PieceTests, getRoute_InvalidMove_ValidAttack) {
	Queen queen(Position("D5"));
	//WHEN_CALLED(queen.isConsistentWithMoveRules(ANY_REF(Position))).Return(false);
	//WHEN_CALLED(queen.isConsistentWithAttackRules(ANY_REF(Position))).Return(true);

	Position p("A1");
	std::vector<Position> dummyRoute;
	dummyRoute.push_back(Position("D1"));
	dummyRoute.push_back(Position("D2"));
	//WHEN_CALLED(p.getSimplestRoute(ANY_REF(Position))).Return(BY_VAL(dummyRoute));

	EXPECT_EQ(dummyRoute, queen.getRoute(Position("F3")));
}

TEST_F(PieceTests, getRoute_InvalidMove_InvalidAttack) {
	Queen queen(Position("D5"));
	//WHEN_CALLED(queen.isConsistentWithMoveRules(ANY_REF(Position))).Return(false);
	//WHEN_CALLED(queen.isConsistentWithAttackRules(ANY_REF(Position))).Return(false);

	EXPECT_ANY_THROW(queen.getRoute(Position("F3")));
}

TEST_F(PieceTests, getAllPossibleMoves_ValidAttack_InvalidMove) {
	Knight knight(Position("D5"));
	//WHEN_CALLED(knight.isConsistentWithMoveRules(ANY_REF(Position))).Return(true);
	//WHEN_CALLED(knight.isConsistentWithAttackRules(ANY_REF(Position))).Return(false);

	EXPECT_EQ(knight.getAllPossibleMoves().size(), 64);
}

TEST_F(PieceTests, getAllPossibleMoves_InvalidAttack_ValidMove) {
	Knight knight(Position("D5"));
	//WHEN_CALLED(knight.isConsistentWithMoveRules(ANY_REF(Position))).Return(false);
	//WHEN_CALLED(knight.isConsistentWithAttackRules(ANY_REF(Position))).Return(true);

	EXPECT_EQ(knight.getAllPossibleMoves().size(), 64);
}

TEST_F(PieceTests, getAllPossibleMoves_InvalidAttack_InvalidMove) {
	Knight knight(Position("D5"));
	//WHEN_CALLED(knight.isConsistentWithMoveRules(ANY_REF(Position))).Return(false);
	//WHEN_CALLED(knight.isConsistentWithAttackRules(ANY_REF(Position))).Return(false);

	EXPECT_TRUE(knight.getAllPossibleMoves().empty());
}
