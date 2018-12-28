#pragma once
#include "Position.h"
#include "Piece.h"

struct SimpleMove {
	SimpleMove(const Position& origin, const Position& destination) : origin_(origin), destination_(destination) {
		if (origin == destination) throw InvalidMoveSpecifiad();
	}
	Position origin_;
	Position destination_;

	class InvalidMoveSpecifiad final : public std::exception {
		const char *what() const noexcept override {
			return "Origin and destination can not be the same position";
		}
	};
};

struct Move {
	Move(const SimpleMove& move, Piece* movedPiece, const bool wasPieceKilled = false) :
		move_(move),
		movedPiece_(movedPiece),
		wasPiecekilled_(wasPieceKilled) {}

	const SimpleMove move_;
	Piece* const movedPiece_;// no ownership
	bool wasPiecekilled_;
};
