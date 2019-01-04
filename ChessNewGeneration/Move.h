#pragma once
#include "Position.h"
#include "Piece.h"

struct SimpleMove {
	SimpleMove(const Position& origin, const Position& destination) : origin_(origin), destination_(destination) {}
	const Position origin_;
	const Position destination_;
};

inline bool operator==(const SimpleMove& lhs, const SimpleMove& rhs) noexcept {
	return lhs.origin_ == rhs.origin_ && lhs.destination_ == rhs.destination_;
}

struct Move {
	Move(SimpleMove move, Piece* const movedPiece, const bool wasFirstMove, const bool wasPieceKilled = false) :
		move_(std::move(move)),
		movedPiece_(movedPiece),
		wasFirstMove_(wasFirstMove),
		wasPieceKilled_(wasPieceKilled) {}

	const SimpleMove move_;
	Piece* const movedPiece_;// no ownership
	const bool wasFirstMove_;
	const bool wasPieceKilled_;
};
