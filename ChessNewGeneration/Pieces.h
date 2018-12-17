#pragma once
#include "Position.h"
#include "Playerh.h"
#include <vector>

class Piece {
protected:
	Position position_;
	const Player owner_;
public:
	constexpr explicit Piece(const Position& pos, const Player owner = Player::White) noexcept : position_(pos), owner_(owner) {}
	Piece(const Piece& original) noexcept = default;
	Piece(Piece&& original) noexcept = default;
	Piece& operator=(const Piece& original) noexcept = default;
	Piece& operator=(Piece&& original) noexcept = default;
	virtual ~Piece() = default;

	auto getPosition() const noexcept {
		return position_;
	}

	auto getOwner() const noexcept {
		return owner_;
	}

	virtual std::vector<Position> getRoute(const Position& destination) const {
		if (!isConsistentWithMoveRules(destination) && !isConsistentWithAttackRules(destination))
			throw InvalidDestination();
		return position_.getSimplestRoute(destination);
	}
	virtual bool isConsistentWithMoveRules(const Position& destPosition) const noexcept = 0;
	virtual bool isConsistentWithAttackRules(const Position& destPosition) const noexcept {
		return isConsistentWithMoveRules(destPosition);
	}
	virtual bool isCheckingCollisions() const noexcept {
		return true;
	}
	virtual void attack(const Position& destPosition) noexcept {
		if (position_ != destPosition && isConsistentWithAttackRules(destPosition))
			position_ = destPosition;
	}
	virtual void move(const Position& destPosition) noexcept {
		if (position_ != destPosition && isConsistentWithMoveRules(destPosition))
			position_ = destPosition;
	}

	class InvalidDestination final : public std::exception {
		const char *what() const noexcept override {
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
};

class Queen final : public Piece {
public:
	constexpr explicit Queen(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return position_.column_ == destPosition.column_ || position_.row_ == destPosition.row_ ||
			std::abs(position_.column_ - destPosition.column_) == std::abs(position_.row_ - destPosition.row_);
	}
};

class Rock final : public Piece {
public:
	constexpr explicit Rock(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return position_.column_ == destPosition.column_ || position_.row_ == destPosition.row_;
	}
};

class Bishop final : public Piece {
public:
	constexpr explicit Bishop(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return std::abs(position_.column_ - destPosition.column_) == std::abs(position_.row_ - destPosition.row_);
	}
};

class Knight final : public Piece {
public:
	constexpr explicit Knight(const Position& pos, const Player owner = Player::White) noexcept : Piece(pos, owner) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return std::abs(position_.column_ - destPosition.column_) == 2 && std::abs(position_.row_ - destPosition.row_) == 1 ||
			std::abs(position_.column_ - destPosition.column_) == 1 && std::abs(position_.row_ - destPosition.row_) == 2;
	}

	bool isCheckingCollisions() const noexcept final {
		return false;
	}

	std::vector<Position> getRoute(const Position& destination) const override {
		return std::vector<Position>();
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
};