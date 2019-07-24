#pragma once
#include "Piece.h"
#include "Player.h"
#include <algorithm>
#include <stack>
#include "Move.h"
#include "ChessPieces.h"

class ChessBoard {	
	ChessPieces pieces_;
	std::vector<SimpleMove> possibleMoves;
	std::stack<Move> moves_;
public:
	ChessBoard() {
		for (const auto& origin : Board::getAllPossiblePositions())
			for (const auto& destination : Board::getAllPossiblePositions())
				if (origin != destination) possibleMoves.emplace_back(origin, destination);
	}
	ChessBoard(const ChessBoard& cp) = delete;
	ChessBoard& operator=(const ChessBoard& cp) = delete;
	ChessBoard(ChessBoard&& mv) = delete;
	ChessBoard& operator=(ChessBoard&& mv) = delete;
	~ChessBoard() = default;
	auto findKing(const Player& owner) const noexcept {
		return std::find_if(pieces_.begin(), pieces_.end(), [&owner](const auto& piece) {
			return  piece->getOwner() == owner && piece->isKing();
		});
	}

	auto getPieceByPosition(const Position& position) const noexcept {
		return std::find_if(pieces_.begin(), pieces_.end(), [&position](const auto& piece) {
			return piece->isAlive() && piece->getPosition() == position; 
		});
	}

    std::array<std::unique_ptr<Piece>, 32>::iterator getPieceByPosition(const Position& position) noexcept {
        return std::find_if(pieces_.begin(), pieces_.end(), [&position](auto& piece) {
            return piece->isAlive() && piece->getPosition() == position;
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

	void doMove(const Move& move) {
		move.execute();
		moves_.push(move);
	}

	void undoMove() noexcept {
		if (wasThereAnyMove()) {
			getLastMove().undo();
			moves_.pop();
		}	
	}

	auto& getPieces() noexcept {
		return pieces_;
	}

	const auto& getPieces() const noexcept {
		return pieces_;
	}

	auto getPiecesCopy() const noexcept {
		return pieces_.getPiecesCopy();
	}

	Move getLastMove() const {
		return moves_.top();
	}

	bool wasThereAnyMove() const noexcept {
		return !moves_.empty();
	}

	std::vector<SimpleMove> getAllPossibleMoves() const noexcept {
		return possibleMoves;
	}
};
