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

	virtual bool isConsistentWithMoveRules(const Position& destPosition) const = 0;
	virtual bool isConsistentWithAttackRules(const Position& destPosition) const noexcept = 0;
	void attack(const Position& destPosition) noexcept {
		if (position_ != destPosition && isConsistentWithAttackRules(destPosition))
			position_ = destPosition;
	}
	void move(const Position& destPosition) noexcept {
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
	bool isConsistentWithAttackRules(const Position& destPosition) const noexcept final {
		return isConsistentWithMoveRules(destPosition);
	}
};