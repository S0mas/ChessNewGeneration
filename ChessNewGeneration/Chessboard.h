#pragma once
#include "Pieces.h"
#include "Player.h"
#include <algorithm>
#include <stack>

struct SimpleMove {
	Position origin_;
	Position destination_;
};

struct Move {
	Move(const Position& origin, const Position destination, Piece* movedPiece, const bool wasPieceKilled = false) :
		origin_(origin),
		destination_(destination),
		movedPiece_(movedPiece),
		wasPiecekilled_(wasPieceKilled) {}

	const Position origin_;
	const Position destination_;
	Piece* const movedPiece_;// no ownership
	bool wasPiecekilled_;
};

class Pieces {
	std::array<std::unique_ptr<Piece>, 32> pieces_;
	int deadPiecesCounter = 0;

	void buildPieces(const Player& owner, int& posInArray) {
		std::string rowNumber = (owner == Player::White) ? "1" : "8";
		pieces_.at(posInArray++) = std::make_unique<King>(Position("E" + rowNumber));
		pieces_.at(posInArray++) = std::make_unique<Queen>(Position("D" + rowNumber));
		pieces_.at(posInArray++) = std::make_unique<Rock>(Position("A" + rowNumber));
		pieces_.at(posInArray++) = std::make_unique<Rock>(Position("H" + rowNumber));
		pieces_.at(posInArray++) = std::make_unique<Knight>(Position("B" + rowNumber));
		pieces_.at(posInArray++) = std::make_unique<Knight>(Position("G" + rowNumber));
		pieces_.at(posInArray++) = std::make_unique<Bishop>(Position("C" + rowNumber));
		pieces_.at(posInArray++) = std::make_unique<Bishop>(Position("F" + rowNumber));

		rowNumber = (owner == Player::White) ? "2" : "7";
		for (const auto& column : Position::getAllColumns()) {
			pieces_.at(posInArray++) = std::make_unique<Bishop>(Position(column + rowNumber));
		}
	}
public:
	Pieces() {
		auto posInArray = 0;
		buildPieces(Player::White, posInArray);
		buildPieces(Player::Black, posInArray);
	};

	const auto cbegin() const noexcept {
		return pieces_.cbegin();
	}
	const auto cend() const noexcept {
		return pieces_.cend() - deadPiecesCounter;
	}

	auto begin() noexcept {
		return pieces_.begin();
	}

	auto end() noexcept {
		return pieces_.end() - deadPiecesCounter;
	}

	void killPiece(std::array<std::unique_ptr<Piece>, 32>::iterator& pieceToKill) {
		++deadPiecesCounter;
		pieceToKill->swap(pieces_.at(pieces_.size()-deadPiecesCounter));
	}

	void resurectLastKilledPiece() {
		--deadPiecesCounter;
	}
};

class ChessBoard {	
	Pieces pieces_;
	std::stack<Move> moves_;
public:
	auto findKing(const Player& owner) const noexcept {
		return  std::find_if(pieces_.cbegin(), pieces_.cend(), [&owner](const auto& piece) {
			return  piece->getOwner() == owner && piece->isKing();
		});
	}

	auto getPieceByPosition(const Position& position) const noexcept {
		return std::find_if(pieces_.cbegin(), pieces_.cend(), [&position](const auto& piece) {
			return piece->getPosition() == position; 
		});
	}

	const auto notFound() const noexcept {
		return pieces_.cend();
	}

	bool isPositionOccupied(const Position& position) const noexcept {
		return notFound() != getPieceByPosition(position);
	}

	void doMove(const SimpleMove& simpleMove) {
		const auto& origin = simpleMove.origin_;
		const auto& destination = simpleMove.destination_;
		Piece* movedPiece = getPieceByPosition(origin)->get();
		movedPiece->setPosition(destination);

		auto pieceToKill = std::find_if(pieces_.begin(), pieces_.end(), [&destination](const auto& piece) {
			return piece->getPosition() == destination;
		});
		if (pieceToKill == notFound())
			moves_.emplace(Move(origin, destination, movedPiece));
		else {
			pieces_.killPiece(pieceToKill);
			moves_.emplace(Move(origin, destination, movedPiece, true));
		}
	}

	void undoMove() {
		if(!moves_.empty()) {
			const auto& moveToUndo = moves_.top();
			moveToUndo.movedPiece_->setPosition(moveToUndo.origin_);
			if (moveToUndo.wasPiecekilled_)
				pieces_.resurectLastKilledPiece();
			moves_.pop();
		}
	}

	auto& getPieces() noexcept {
		return pieces_;
	}

	const auto& getPieces() const noexcept {
		return pieces_;
	}
};