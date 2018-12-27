#include "pch.h"
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

TEST_F(PieceTests, positionsUpdateAfterValidMove) {
	PieceMock fakePiece;
	const Position newPosition("B4");
	fakePiece.move(Position(newPosition));
	EXPECT_EQ(fakePiece.getPosition(), newPosition);
}

TEST_F(PieceTests, positionsNotUpdateAfterInvalidMove) {
	PieceMock fakePiece;
	const auto oldPosition = fakePiece.getPosition();
	fakePiece.move(Position("D4"));
	EXPECT_EQ(fakePiece.getPosition(), oldPosition);
}
TEST_F(PieceTests, positionsUpdateAfterValidAttack) {
	PieceMock fakePiece;
	const Position newPosition("B4");
	fakePiece.attack(Position(newPosition));
	EXPECT_EQ(fakePiece.getPosition(), newPosition);
}

TEST_F(PieceTests, positionsNotUpdateAfterInvalidAttack) {
	PieceMock fakePiece;
	const auto oldPosition = fakePiece.getPosition();
	fakePiece.attack(Position("D4"));
	EXPECT_EQ(fakePiece.getPosition(), oldPosition);
}

TEST_F(PieceTests, isKing) {
	const King king(Position("A1"));

	EXPECT_TRUE(king.isKing());
}

TEST_F(PieceTests, isNotKing) {
	const Bishop bishop(Position("A1"));

	EXPECT_FALSE(bishop.isKing());
}