#pragma once
#include "Piece.h"
#include "Player.h"
#include <algorithm>
#include <stack>
#include "Move.h"
#include "ChessPieces.h"

class ChessBoard {	
	ChessPieces pieces_;
	std::stack<Move> moves_;
public:
	ChessBoard() = default;
	ChessBoard(const ChessBoard& cp) = default;
	auto findKing(const Player& owner) const noexcept {
		return std::find_if(pieces_.begin(), pieces_.end(), [&owner](const auto& piece) {
			return  piece->getOwner() == owner && piece->isKing();
		});
	}

	auto getPieceByPosition(const Position& position) const noexcept {
		return std::find_if(pieces_.begin(), pieces_.end(), [&position](const auto& piece) {
			return piece->getPosition() == position; 
		});
	}

	auto notFound() const noexcept {
		return pieces_.end();
	}

	bool isPositionOccupied(const Position& position) const noexcept {
		return notFound() != getPieceByPosition(position);
	}

	bool isThereCollision(const std::vector<Position>& route) const noexcept {
		return route.end() != std::find_if(route.begin(), route.end(), [this](const Position& position){ return isPositionOccupied(position);});
	}

	void doMove(const SimpleMove& move) {
		const auto& pieceToKill = std::find_if(pieces_.begin(), pieces_.end(), [&move](const auto& piece) {
			return piece->getPosition() == move.destination_;
		});

		auto movedPiece = getPieceByPosition(move.origin_)->get();
		const auto wasFirstMove = movedPiece->hasNotMoved();
		movedPiece->setPosition(move.destination_);
		const auto& pieceToKillPosition = std::distance(pieces_.begin(), pieceToKill);
		pieces_.killPiece(static_cast<int>(pieceToKillPosition));
		addMove(Move(move, movedPiece, wasFirstMove, pieceToKill != notFound()));
	}

	bool undoMove() {
		if(!moves_.empty()) {
			const auto& moveToUndo = moves_.top();
			moveToUndo.movedPiece_->setPosition(moveToUndo.move_.origin_);
			moveToUndo.movedPiece_->setFirstMove(moveToUndo.wasFirstMove_);
			if (moveToUndo.wasPieceKilled_)
				pieces_.resurrectLastKilledPiece();
			moves_.pop();
			return true;
		}
		return false;
	}

	void addMove(const Move& move) noexcept {
		moves_.push(move);
	}

	auto& getPieces() noexcept {
		return pieces_;
	}

	const auto& getPieces() const noexcept {
		return pieces_;
	}

	void reset() noexcept {
		while (undoMove()) {};
	}

	Move getLastMove() const {
		return moves_.top();
	}

	bool wasThereAnyMoves() const {
		return moves_.empty();
	}
};
