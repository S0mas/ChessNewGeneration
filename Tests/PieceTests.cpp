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

TEST(PieceTests, positionsUpdateAfterValidMove) {
	PieceMock fakePiece;
	const Position newPosition("B4");
	fakePiece.move(Position(newPosition));
	EXPECT_EQ(fakePiece.getPosition(), newPosition);
}

TEST(PieceTests, positionsNotUpdateAfterInvalidMove) {
	PieceMock fakePiece;
	const auto oldPosition = fakePiece.getPosition();
	fakePiece.move(Position("D4"));
	EXPECT_EQ(fakePiece.getPosition(), oldPosition);
}
TEST(PieceTests, positionsUpdateAfterValidAttack) {
	PieceMock fakePiece;
	const Position newPosition("B4");
	fakePiece.attack(Position(newPosition));
	EXPECT_EQ(fakePiece.getPosition(), newPosition);
}

TEST(PieceTests, positionsNotUpdateAfterInvalidAttack) {
	PieceMock fakePiece;
	const auto oldPosition = fakePiece.getPosition();
	fakePiece.attack(Position("D4"));
	EXPECT_EQ(fakePiece.getPosition(), oldPosition);
}

TEST(PieceTests, isKing) {
	std::unique_ptr<Piece> king = std::make_unique<King>(Position("A1"));
	std::unique_ptr<Piece> bishop = std::make_unique<Bishop>(Position("A1"));

	EXPECT_TRUE(king->isKing());
	EXPECT_FALSE(bishop->isKing());
}