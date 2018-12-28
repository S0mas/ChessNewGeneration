#pragma once
#include "Pieces.h"
#include "Player.h"
#include <algorithm>
#include <stack>
#include "ChessboardPrinter.h"
#include <iostream>

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

class Pieces {
	std::array<std::unique_ptr<Piece>, 32> pieces_;
	int deadPiecesCounter = 0;

	void buildPieces(const Player& owner, int& posInArray) {
		std::string rowNumber = (owner == Player::White) ? "1" : "8";
		pieces_.at(posInArray++) = std::make_unique<King>(Position("E" + rowNumber), owner);
		pieces_.at(posInArray++) = std::make_unique<Queen>(Position("D" + rowNumber), owner);
		pieces_.at(posInArray++) = std::make_unique<Rook>(Position("A" + rowNumber), owner);
		pieces_.at(posInArray++) = std::make_unique<Rook>(Position("H" + rowNumber), owner);
		pieces_.at(posInArray++) = std::make_unique<Knight>(Position("B" + rowNumber), owner);
		pieces_.at(posInArray++) = std::make_unique<Knight>(Position("G" + rowNumber), owner);
		pieces_.at(posInArray++) = std::make_unique<Bishop>(Position("C" + rowNumber), owner);
		pieces_.at(posInArray++) = std::make_unique<Bishop>(Position("F" + rowNumber), owner);

		rowNumber = (owner == Player::White) ? "2" : "7";
		for (const auto& column : Position::getAllColumns()) {
			pieces_.at(posInArray++) = std::make_unique<Pawn>(Position(column + rowNumber), owner);
		}
	}
public:
	Pieces() {
		auto posInArray = 0;
		buildPieces(Player::White, posInArray);
		buildPieces(Player::Black, posInArray);
	};

	auto cbegin() const noexcept {
		return pieces_.cbegin();
	}
	auto cend() const noexcept {
		return pieces_.cend() - deadPiecesCounter;
	}

	auto begin() noexcept {
		return pieces_.begin();
	}

	auto end() noexcept {
		return pieces_.end() - deadPiecesCounter;
	}

	void killPiece(std::array<std::unique_ptr<Piece>, 32>::iterator& pieceToKill) noexcept {
		if(pieceToKill != cend()) {
			++deadPiecesCounter;
			pieceToKill->swap(pieces_.at(pieces_.size() - deadPiecesCounter));
		}
	}

	void resurectLastKilledPiece() noexcept {
		--deadPiecesCounter;
	}
};

class ChessBoard {	
	Pieces pieces_;
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
				pieces_.resurectLastKilledPiece();
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

	std::string toString() const noexcept {
		std::string result;
		for (auto i = 8; i > 0; --i) {
			Row row(i);
			for (auto j = 0; j < 8; ++j) {
				const auto& piece = getPieceByPosition(Position(j, i - 1));
				if (piece != notFound())
					row.fields[j].piece_ = (*piece)->toString();
				else
					row.fields[j].piece_ = row.fields[j].spaceChar;
			}
			result += row.toString();			
		}
		result += "  A  B  C  D  E  F  G  H  \n";
		return result;
	}
};