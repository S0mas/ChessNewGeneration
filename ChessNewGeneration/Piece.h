#pragma once
#include "ChessInterface.h"
#include "Position.h"
#include "Player.h"
#include <vector>
#include <memory>

class Piece {
protected:
	Position position_;
	const Player owner_;
	bool firstMove_ = true;
	bool live = true;
public:
	constexpr explicit Piece(const Position& pos, const Player owner = Player::White) noexcept : position_(pos), owner_(owner) {}
	Piece(const Piece& original) noexcept = default;
	Piece(Piece&& original) noexcept = default;
	Piece& operator=(const Piece& original) noexcept = delete;
	Piece& operator=(Piece&& original) noexcept = delete;
	virtual ~Piece() = default;

	auto getPosition() const noexcept {
		return position_;
	}

	auto getOwner() const noexcept {
		return owner_;
	}

	auto hasNotMoved() const noexcept {
		return firstMove_;
	}

	void setFirstMove(const bool newValue) noexcept {
		firstMove_ = newValue;
	}

	void kill() noexcept {
		live = false;
	}

	void resurrect() noexcept {
		live = true;
	}

	bool isAlive() const noexcept {
		return live;
	}

    virtual ChessInterface::PieceType getType() const noexcept = 0;

	virtual void setPosition(const Position& destination) noexcept {
		position_ = destination;
		firstMove_ = false;
	}

	virtual std::vector<Position> getRoute(const Position& destination) const {
		if (!isConsistentWithMoveRules(destination) && !isConsistentWithAttackRules(destination))
			throw InvalidDestination();
		return position_.getSimplestRoute(destination);
	}

	virtual bool isKing() const noexcept {
		return false;
	}

	virtual bool isPawn() const noexcept {
		return false;
	}

	virtual std::unique_ptr<Piece> clone() const = 0;

	virtual bool isConsistentWithMoveRules(const Position& destPosition) const noexcept = 0;
	virtual bool isConsistentWithAttackRules(const Position& destPosition) const noexcept {
		return isConsistentWithMoveRules(destPosition);
	}

	std::vector<Position> getAllPossibleMoves() const noexcept {
		std::vector<Position> possibleMoves;
		possibleMoves.reserve(20);
		for (const auto& position : Board::getAllPossiblePositions())
			if (isConsistentWithAttackRules(position) || isConsistentWithMoveRules(position))
				possibleMoves.push_back(position);
		return possibleMoves;
	}

	class InvalidDestination final : public std::exception {
		const char* what() const noexcept final {
			return "Invalid destination specified";
		}
	};
};

class King final : public Piece {
public:
	constexpr explicit King(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return std::abs(position_.column_ - destPosition.column_) <=1 && + std::abs(position_.row_ - destPosition.row_) <= 1;
	}

	bool isKing() const noexcept final {
		return true;
	}

	std::unique_ptr<Piece> clone() const final {
		return std::make_unique<King>(*this);
	}

    virtual ChessInterface::PieceType getType() const noexcept {
        return getOwner() == Player::White ? ChessInterface::PieceType::WhiteKing : ChessInterface::PieceType::BlackKing;
	}
};

class Queen final : public Piece {
public:
	constexpr explicit Queen(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return position_.column_ == destPosition.column_ || position_.row_ == destPosition.row_ ||
			std::abs(position_.column_ - destPosition.column_) == std::abs(position_.row_ - destPosition.row_);
	}

	std::unique_ptr<Piece> clone() const final {
		return std::make_unique<Queen>(*this);
	}

    virtual ChessInterface::PieceType getType() const noexcept {
        return getOwner() == Player::White ? ChessInterface::PieceType::WhiteQueen : ChessInterface::PieceType::BlackQueen;
	}
};

class Rook final : public Piece {
public:
	constexpr explicit Rook(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return position_.column_ == destPosition.column_ || position_.row_ == destPosition.row_;
	}

	void castle() noexcept {
		if (position_ == Position("A1") || position_ == Position("A8"))
			position_.column_ += 3;
		else
			position_.column_ -= 2;
		firstMove_ = false;
	}

	void undoCastle() noexcept {
		if (position_ == Position("A1") || position_ == Position("A8"))
			position_.column_ -= 3;
		else
			position_.column_ += 2;
		firstMove_ = true;
	}

	std::unique_ptr<Piece> clone() const final {
		return std::make_unique<Rook>(*this);
	}

    virtual ChessInterface::PieceType getType() const noexcept {
        return getOwner() == Player::White ? ChessInterface::PieceType::WhiteRook : ChessInterface::PieceType::BlackRook;
	}
};

class Bishop final : public Piece {
public:
	constexpr explicit Bishop(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return std::abs(position_.column_ - destPosition.column_) == std::abs(position_.row_ - destPosition.row_);
	}

	std::unique_ptr<Piece> clone() const final {
		return std::make_unique<Bishop>(*this);
	}

    virtual ChessInterface::PieceType getType() const noexcept {
        return getOwner() == Player::White ? ChessInterface::PieceType::WhiteBishop : ChessInterface::PieceType::BlackBishop;
	}
};

class Knight final : public Piece {
public:
	constexpr explicit Knight(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return std::abs(position_.column_ - destPosition.column_) == 2 && std::abs(position_.row_ - destPosition.row_) == 1 ||
			std::abs(position_.column_ - destPosition.column_) == 1 && std::abs(position_.row_ - destPosition.row_) == 2;
	}

	std::vector<Position> getRoute(const Position& destination) const override {
		return std::vector<Position>();
	}

	std::unique_ptr<Piece> clone() const final {
		return std::make_unique<Knight>(*this);
	}

    virtual ChessInterface::PieceType getType() const noexcept {
        return getOwner() == Player::White ? ChessInterface::PieceType::WhiteKnight : ChessInterface::PieceType::BlackKnight;
	}
};

class Pawn final : public Piece {
public:
	constexpr explicit Pawn(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner){}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return position_.column_ == destPosition.column_ && (position_.row_ + static_cast<int>(owner_) == destPosition.row_ || firstMove_ && position_.row_ + 2*static_cast<int>(owner_) == destPosition.row_);
	}

	bool isConsistentWithAttackRules(const Position& destPosition) const noexcept final {
		return position_.row_ + static_cast<int>(owner_) == destPosition.row_ && std::abs(position_.column_ - destPosition.column_) == 1;
	}

	bool isPawn() const noexcept final {
		return true;
	}

    std::unique_ptr<Piece> promote(const ChessInterface::PieceType& type) const noexcept {
        if(type == ChessInterface::WhiteQueen || type == ChessInterface::BlackQueen)
            return std::make_unique<Queen>(getPosition(),getOwner());
        if(type == ChessInterface::WhiteRook || type == ChessInterface::BlackRook)
            return std::make_unique<Rook>(getPosition(),getOwner());
        if(type == ChessInterface::WhiteBishop || type == ChessInterface::BlackBishop)
            return std::make_unique<Bishop>(getPosition(),getOwner());
        return std::make_unique<Knight>(getPosition(),getOwner());
    }

	std::unique_ptr<Piece> clone() const final {
		return std::make_unique<Pawn>(*this);
	}

    virtual ChessInterface::PieceType getType() const noexcept {
        return getOwner() == Player::White ? ChessInterface::PieceType::WhitePawn : ChessInterface::PieceType::BlackPawn;
	}
};
