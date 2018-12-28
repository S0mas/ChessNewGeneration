#pragma once
#include <array>
#include "Piece.h"

class ChessPieces {
	std::array<std::unique_ptr<Piece>, 32> pieces_ {
		std::make_unique<Rook>(Position("A1")),
		std::make_unique<Knight>(Position("B1")),
		std::make_unique<Bishop>(Position("C1")),
		std::make_unique<Queen>(Position("D1")),
		std::make_unique<King>(Position("E1")),
		std::make_unique<Bishop>(Position("F1")),
		std::make_unique<Knight>(Position("G1")),
		std::make_unique<Rook>(Position("H1")),
		std::make_unique<Pawn>(Position("A2")),
		std::make_unique<Pawn>(Position("B2")),
		std::make_unique<Pawn>(Position("C2")),
		std::make_unique<Pawn>(Position("D2")),
		std::make_unique<Pawn>(Position("E2")),
		std::make_unique<Pawn>(Position("F2")),
		std::make_unique<Pawn>(Position("G2")),
		std::make_unique<Pawn>(Position("H2")),

		std::make_unique<Rook>(Position("A8"), Player::Black),
		std::make_unique<Knight>(Position("B8"), Player::Black),
		std::make_unique<Bishop>(Position("C8"), Player::Black),
		std::make_unique<Queen>(Position("D8"), Player::Black),
		std::make_unique<King>(Position("E8"), Player::Black),
		std::make_unique<Bishop>(Position("F8"), Player::Black),
		std::make_unique<Knight>(Position("G8"), Player::Black),
		std::make_unique<Rook>(Position("H7"), Player::Black),
		std::make_unique<Pawn>(Position("A7"), Player::Black),
		std::make_unique<Pawn>(Position("B7"), Player::Black),
		std::make_unique<Pawn>(Position("C7"), Player::Black),
		std::make_unique<Pawn>(Position("D7"), Player::Black),
		std::make_unique<Pawn>(Position("E7"), Player::Black),
		std::make_unique<Pawn>(Position("F7"), Player::Black),
		std::make_unique<Pawn>(Position("G7"), Player::Black),
		std::make_unique<Pawn>(Position("H7"), Player::Black),
	};
	int deadPiecesCounter = 0;
public:
	ChessPieces() noexcept = default;
	auto begin() const noexcept {
		return pieces_.cbegin();
	}
	auto end() const noexcept {
		return pieces_.cend() - deadPiecesCounter;
	}

	auto begin() noexcept {
		return pieces_.begin();
	}

	auto end() noexcept {
		return pieces_.end() - deadPiecesCounter;
	}

	void killPiece(std::array<std::unique_ptr<Piece>, 32>::iterator& pieceToKill) noexcept {
		if (pieceToKill != end()) {
			++deadPiecesCounter;
			pieceToKill->swap(pieces_.at(pieces_.size() - deadPiecesCounter));
		}
	}

	void resurrectLastKilledPiece() noexcept {
		--deadPiecesCounter;
	}
};
