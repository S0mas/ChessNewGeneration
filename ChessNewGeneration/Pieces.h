#pragma once
#include "Position.h"
#include "Player.h"
#include <vector>
class King;
class Piece {
protected:
	Position position_;
	const Player owner_;
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

	void setPosition(const Position& destination) {
		position_ = destination;
	}

	//TODO:Tests
	virtual std::vector<Position> getRoute(const Position& destination) const {
		if (!isConsistentWithMoveRules(destination) && !isConsistentWithAttackRules(destination))
			throw InvalidDestination();
		return position_.getSimplestRoute(destination);
	}

	virtual bool isKing() const noexcept {
		return false;
	}

	virtual bool isConsistentWithMoveRules(const Position& destPosition) const noexcept = 0;
	virtual bool isConsistentWithAttackRules(const Position& destPosition) const noexcept {
		return isConsistentWithMoveRules(destPosition);
	}
	//TODO:Tests
	std::vector<Position> getAllPossibleMoves() const noexcept {
		std::vector<Position> possibleMoves;
		possibleMoves.reserve(20);
		for (const auto& position : Position::getAllPossiblePositions())
			if (isConsistentWithAttackRules(position) || isConsistentWithMoveRules(position))
				possibleMoves.push_back(position);
		return possibleMoves;
	}

	virtual void attack(const Position& destPosition) noexcept {
		if (position_ != destPosition && isConsistentWithAttackRules(destPosition))
			position_ = destPosition;
	}

	virtual void move(const Position& destPosition) noexcept {
		if (position_ != destPosition && isConsistentWithMoveRules(destPosition))
			position_ = destPosition;
	}
	//TODO:Tests
	virtual std::string toString() const noexcept { return "ABSTRACT_PIECE"; };

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
		return std::abs(position_.column_ - destPosition.column_) <= 1 && std::abs(position_.row_ - destPosition.row_) <= 1;
	}

	std::string toString() const noexcept final {
		return owner_ == Player::White ? std::string("wK") : std::string("bK");
	}

	bool isKing() const noexcept final {
		return true;
	}
};

class Queen final : public Piece {
public:
	constexpr explicit Queen(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return position_.column_ == destPosition.column_ || position_.row_ == destPosition.row_ ||
			std::abs(position_.column_ - destPosition.column_) == std::abs(position_.row_ - destPosition.row_);
	}

	std::string toString() const noexcept final {
		return owner_ == Player::White ? std::string("wQ") : std::string("bQ");
	}
};

class Rook final : public Piece {
public:
	constexpr explicit Rook(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return position_.column_ == destPosition.column_ || position_.row_ == destPosition.row_;
	}

	std::string toString() const noexcept final {
		return owner_ == Player::White ? std::string("wR") : std::string("bR");
	}
};

class Bishop final : public Piece {
public:
	constexpr explicit Bishop(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return std::abs(position_.column_ - destPosition.column_) == std::abs(position_.row_ - destPosition.row_);
	}

	std::string toString() const noexcept final {
		return owner_ == Player::White ? std::string("wB") : std::string("bB");
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

	std::string toString() const noexcept final {
		return owner_ == Player::White ? std::string("wN") : std::string("bN");
	}
};

class Pawn final : public Piece {
	bool firstMove_ = true;
public:
	constexpr explicit Pawn(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner){}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return position_.column_ == destPosition.column_ && (position_.row_ + static_cast<int>(owner_) == destPosition.row_ || firstMove_ && position_.row_ + 2*static_cast<int>(owner_) == destPosition.row_);
	}

	bool isConsistentWithAttackRules(const Position& destPosition) const noexcept final {
		return position_.row_ + static_cast<int>(owner_) == destPosition.row_ && std::abs(position_.column_ - destPosition.column_) == 1;
	}

	void attack(const Position& destPosition) noexcept final {
		if (position_ != destPosition && isConsistentWithAttackRules(destPosition)) {
			position_ = destPosition;
			firstMove_ = false;
		}
	}

	void move(const Position& destPosition) noexcept final {
		if (position_ != destPosition && isConsistentWithMoveRules(destPosition)) {
			position_ = destPosition;
			firstMove_ = false;
		}
	}

	std::string toString() const noexcept final {
		return owner_ == Player::White ? std::string("wP") : std::string("bP");
	}
};