#pragma once
#include "Position.h"

class Piece {
protected:
	Position position_;
public:
	constexpr explicit Piece(const Position& pos) noexcept : position_(pos) {}
	Piece(const Piece& original) noexcept = default;
	Piece(Piece&& original) noexcept = default;
	Piece& operator=(const Piece& original) noexcept = default;
	Piece& operator=(Piece&& original) noexcept = default;
	virtual ~Piece() = default;

	auto getPosition() const noexcept {
		return position_;
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
};

class King final : public Piece {
public:
	constexpr explicit King(const Position& pos) noexcept : Piece(pos) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return std::abs(position_.column_ - destPosition.column_) <= 1 && std::abs(position_.row_ - destPosition.row_) <= 1;
	}
};

class Queen final : public Piece {
public:
	constexpr explicit Queen(const Position& pos) noexcept : Piece(pos) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return position_.column_ == destPosition.column_ || position_.row_ == destPosition.row_ ||
			std::abs(position_.column_ - destPosition.column_) == std::abs(position_.row_ - destPosition.row_);
	}
};

class Rock final : public Piece {
public:
	constexpr explicit Rock(const Position& pos) noexcept : Piece(pos) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return position_.column_ == destPosition.column_ || position_.row_ == destPosition.row_;
	}
};

class Bishop final : public Piece {
public:
	constexpr explicit Bishop(const Position& pos) noexcept : Piece(pos) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return std::abs(position_.column_ - destPosition.column_) == std::abs(position_.row_ - destPosition.row_);
	}
};

class Knight final : public Piece {
public:
	constexpr explicit Knight(const Position& pos) noexcept : Piece(pos) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return std::abs(position_.column_ - destPosition.column_) == 2 && std::abs(position_.row_ - destPosition.row_) == 1 ||
			std::abs(position_.column_ - destPosition.column_) == 1 && std::abs(position_.row_ - destPosition.row_) == 2;
	}

	bool isCheckingCollisions() const noexcept final {
		return false;
	}
};

class Pawn final : public Piece {
	bool firstMove_ = true;
public:
	constexpr explicit Pawn(const Position& pos) noexcept : Piece(pos) {}
	bool isConsistentWithMoveRules(const Position& destPosition) const noexcept final {
		return position_.column_ == destPosition.column_ && (position_.row_ + 1 == destPosition.row_ || firstMove_ && position_.row_ + 2 == destPosition.row_);
	}

	bool isConsistentWithAttackRules(const Position& destPosition) const noexcept final {
		return position_.row_ + 1 == destPosition.row_ && std::abs(position_.column_ - destPosition.column_) == 1;
	}

	void attack(const Position& destPosition) noexcept {
		if (position_ != destPosition && isConsistentWithAttackRules(destPosition)) {
			position_ = destPosition;
			firstMove_ = false;
		}
	}
	void move(const Position& destPosition) noexcept {
		if (position_ != destPosition && isConsistentWithMoveRules(destPosition)) {
			position_ = destPosition;
			firstMove_ = false;
		}
	}
};