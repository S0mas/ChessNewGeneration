#pragma once
#include "Piece.h"

struct SimpleMove {
	SimpleMove(const Position& origin, const Position& destination) : origin_(origin), destination_(destination) {}
	const Position origin_;
	const Position destination_;
};

inline bool operator==(const SimpleMove& lhs, const SimpleMove& rhs) noexcept {
	return lhs.origin_ == rhs.origin_ && lhs.destination_ == rhs.destination_;
}

class Move {
protected:
	Piece* subject = nullptr;
	Piece* target = nullptr;
	Rook* castlingTower = nullptr;

    SimpleMove move;
	bool wasFirstMove_ = false;
	bool wasPieceKilled_ = false;
public:
	enum MoveType {
		Default = 0,
		Promotion,
		EnPassant,
		Castle,
		Invalid
	};
	MoveType type;
	Move(Piece* subject, const SimpleMove& move, Piece* target = nullptr, const MoveType type = MoveType::Default, Rook* castlingTower = nullptr)
		: subject(subject), move(move), target(target), type(type), castlingTower(castlingTower) {
		wasFirstMove_ = subject->hasNotMoved();
		wasPieceKilled_ = target != nullptr;
	}
	Move() : move({ Position("A1"), Position("A1") }), type(MoveType::Invalid) {}

	bool operator==(const SimpleMove& other) const noexcept {
		return move == other;
	}

	const Piece* getSubject() const noexcept {
		return subject;
	}

	SimpleMove getSimpleMove() const noexcept {
		return move;
	}

	void execute() const noexcept {
		subject->setPosition(move.destination_);
		if (wasPieceKilled_)
			target->kill();
		executeAdditionalEffects();
	}

	void undo() const noexcept {
		subject->setPosition(move.origin_);
		subject->setFirstMove(wasFirstMove_);
		if (wasPieceKilled_)
			target->resurrect();
		undoAdditionalEffects();
	}

	void executeAdditionalEffects() const noexcept {
		if (type == MoveType::Castle)
			castlingTower->castle();
	}

	void undoAdditionalEffects() const noexcept {
		if (type == MoveType::Castle)
			castlingTower->undoCastle();
	}
	static Move invalid() noexcept {
		return Move();
	}
};

