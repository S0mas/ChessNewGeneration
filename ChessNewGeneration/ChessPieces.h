#pragma once
#include <array>
#include "Piece.h"

class ChessPieces {
	std::array<std::unique_ptr<Piece>, 32> pieces_;
		
	int deadPiecesCounter = 0;
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
	ChessPieces(const ChessPieces& cp) {
		for (auto i = 0; i < pieces_.size(); ++i)
			pieces_[i] = cp.pieces_[i]->clone();
		deadPiecesCounter = cp.deadPiecesCounter;
	}

	ChessPieces(ChessPieces&& cp) noexcept {
		std::swap(pieces_, cp.pieces_);
		deadPiecesCounter = cp.deadPiecesCounter;
	}

	ChessPieces& operator=(const ChessPieces& cp) = delete;
	ChessPieces& operator=(ChessPieces&& cp) = delete;

	auto begin() const noexcept {
		return pieces_.cbegin();
	}
	auto end() const noexcept {
		return pieces_.cend() - deadPiecesCounter;
	}

	void killPiece(const int pieceToKillPosition) noexcept {

		if (pieceToKillPosition < piecesAlive()) {
			++deadPiecesCounter;
			std::swap(pieces_.at(pieceToKillPosition), pieces_.at(pieces_.size() - deadPiecesCounter));
		}
	}

	void resurrectLastKilledPiece() noexcept {
		--deadPiecesCounter;
	}

	int piecesAlive() const noexcept {
		return pieces_.size() - deadPiecesCounter;
	}

	auto getPiecesCopy() const noexcept {
		std::vector<std::unique_ptr<Piece>> piecesCopy;
		for (const auto& piece : *this)
			piecesCopy.push_back(piece->clone());

		return piecesCopy;
	}
};
