#pragma once
#include <array>
#include "Piece.h"

class ChessPieces {
	std::array<std::unique_ptr<Piece>, 32> pieces_;
public:
	ChessPieces() noexcept {
		auto i = 0;
		pieces_[i++] = std::make_unique<Rook>(Position("A1"));
		pieces_[i++] = std::make_unique<Knight>(Position("B1"));
		pieces_[i++] = std::make_unique<Bishop>(Position("C1"));
		pieces_[i++] = std::make_unique<Queen>(Position("D1"));
		pieces_[i++] = std::make_unique<King>(Position("E1"));
		pieces_[i++] = std::make_unique<Bishop>(Position("F1"));
		pieces_[i++] = std::make_unique<Knight>(Position("G1"));
		pieces_[i++] = std::make_unique<Rook>(Position("H1"));
		pieces_[i++] = std::make_unique<Pawn>(Position("A2"));
		pieces_[i++] = std::make_unique<Pawn>(Position("B2"));
		pieces_[i++] = std::make_unique<Pawn>(Position("C2"));
		pieces_[i++] = std::make_unique<Pawn>(Position("D2"));
		pieces_[i++] = std::make_unique<Pawn>(Position("E2"));
		pieces_[i++] = std::make_unique<Pawn>(Position("F2"));
		pieces_[i++] = std::make_unique<Pawn>(Position("G2"));
		pieces_[i++] = std::make_unique<Pawn>(Position("H2"));

		pieces_[i++] = std::make_unique<Rook>(Position("A8"), Player::Black);
		pieces_[i++] = std::make_unique<Knight>(Position("B8"), Player::Black);
		pieces_[i++] = std::make_unique<Bishop>(Position("C8"), Player::Black);
		pieces_[i++] = std::make_unique<Queen>(Position("D8"), Player::Black);
		pieces_[i++] = std::make_unique<King>(Position("E8"), Player::Black);
		pieces_[i++] = std::make_unique<Bishop>(Position("F8"), Player::Black);
		pieces_[i++] = std::make_unique<Knight>(Position("G8"), Player::Black);
		pieces_[i++] = std::make_unique<Rook>(Position("H8"), Player::Black);
		pieces_[i++] = std::make_unique<Pawn>(Position("A7"), Player::Black);
		pieces_[i++] = std::make_unique<Pawn>(Position("B7"), Player::Black);
		pieces_[i++] = std::make_unique<Pawn>(Position("C7"), Player::Black);
		pieces_[i++] = std::make_unique<Pawn>(Position("D7"), Player::Black);
		pieces_[i++] = std::make_unique<Pawn>(Position("E7"), Player::Black);
		pieces_[i++] = std::make_unique<Pawn>(Position("F7"), Player::Black);
		pieces_[i++] = std::make_unique<Pawn>(Position("G7"), Player::Black);
		pieces_[i++] = std::make_unique<Pawn>(Position("H7"), Player::Black);
	}
	~ChessPieces() = default;
	ChessPieces(const ChessPieces& cp) = delete;
	ChessPieces(ChessPieces&& cp) = delete;
	ChessPieces& operator=(const ChessPieces& cp) = delete;
	ChessPieces& operator=(ChessPieces&& cp) = delete;

	auto begin() const noexcept {
		return pieces_.begin();
	}
	auto end() const noexcept {
		return pieces_.end();
	}

    auto begin() noexcept {
        return pieces_.begin();
    }
    auto end() noexcept {
        return pieces_.end();
    }

	auto getPiecesCopy() const noexcept {
		std::vector<std::unique_ptr<Piece>> piecesCopy;
		piecesCopy.reserve(32);
		for (const auto& piece : pieces_)
			if(piece->isAlive()) piecesCopy.push_back(piece->clone());
		return piecesCopy;
	}
};
