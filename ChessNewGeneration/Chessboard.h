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
	auto findKing(const Player& owner) const noexcept {
		return std::find_if(pieces_.cbegin(), pieces_.cend(), [&owner](const auto& piece) {
			return  piece->getOwner() == owner && piece->isKing();
		});
	}

	auto getPieceByPosition(const Position& position) const noexcept {
		return std::find_if(pieces_.cbegin(), pieces_.cend(), [&position](const auto& piece) {
			return piece->getPosition() == position; 
		});
	}

	auto notFound() const noexcept {
		return pieces_.cend();
	}

	bool isPositionOccupied(const Position& position) const noexcept {
		return notFound() != getPieceByPosition(position);
	}

	bool isThereCollision(const std::vector<Position>& route) const noexcept {
		return route.end() != std::find_if(route.begin(), route.end(), [this](const Position& position){ return isPositionOccupied(position);});
	}

	void doMove(const SimpleMove& move) {
		auto pieceToKill = std::find_if(pieces_.begin(), pieces_.end(), [&move](const auto& piece) {
			return piece->getPosition() == move.destination_;
		});

		auto movedPiece = getPieceByPosition(move.origin_)->get();
		movedPiece->setPosition(move.destination_);

		pieces_.killPiece(pieceToKill);
		addMove(Move(move, movedPiece, pieceToKill != notFound()));
	}

	void undoMove() {
		if(!moves_.empty()) {
			const auto& moveToUndo = moves_.top();
			moveToUndo.movedPiece_->setPosition(moveToUndo.move_.origin_);
			if (moveToUndo.wasPiecekilled_)
				pieces_.resurrectLastKilledPiece();
			moves_.pop();
		}
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
};
