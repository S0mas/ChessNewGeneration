#pragma once
#include "Pieces.h"
#include <typeindex>
#include <vector>
#include <algorithm>
#include <iostream>

static bool isKing(const Piece& piece) noexcept {
	return typeid(piece) == typeid(King);
}

//Both kings should allways be in the pieces vector
static auto findKing(const Player& playerToVerify, const std::vector<const Piece*>& pieces) noexcept {
	return  *std::find_if(pieces.begin(), pieces.end(), [playerToVerify](const auto& piece) {
		return isKing(*piece) && piece->getOwner() == playerToVerify;
	});
}



static auto getPieceByPosition(const Position& position, const std::vector<const Piece*>& pieces) noexcept {
	return std::find_if(pieces.begin(), pieces.end(), [&position](const Piece* piece) { return piece->getPosition() == position; });
}

static bool isPositionOccupied(const Position& position, const std::vector<const Piece*>& pieces) noexcept {
	return pieces.end() != getPieceByPosition(position, pieces);
}

class Rule {
};

class CollisionRule {
public:
	static bool isThereNoCollisions(const std::vector<Position>& route, const std::vector<const Piece*>& pieces) {
		return route.end() == std::find_if(route.begin(), route.end(), [&pieces](const Position& position) { return isPositionOccupied(position, pieces); });
	}
};

class AttackRule {
public:
	static bool isAttackValidMove(const Piece& attacker, const Piece& target, const std::vector<const Piece*>& pieces) {
		return !isKing(target) && isAttackPossible(attacker, target, pieces);
	}

	static bool isAttackPossible(const Piece& attacker, const Piece& target, const std::vector<const Piece*>& pieces) {
		return attacker.getOwner() != target.getOwner() && attacker.isConsistentWithAttackRules(target.getPosition())
			&& CollisionRule::isThereNoCollisions(attacker.getRoute(target.getPosition()), pieces);
	}
};

class MoveRule {
public:
	static bool isMovePossible(const Piece& mover, const Position& destination, const std::vector<const Piece*>& pieces) {
		return mover.isConsistentWithMoveRules(destination) && !isPositionOccupied(destination, pieces) && CollisionRule::isThereNoCollisions(mover.getRoute(destination), pieces);
	}
};

class CheckRule {
public:
	static bool isThereNoCheck(const Player& playerToVerify, const std::vector<const Piece*>& pieces) {
		const auto& king = findKing(playerToVerify, pieces);

		return std::find_if(pieces.begin(), pieces.end(), [king, &pieces](const auto& piece) {
			return AttackRule::isAttackPossible(*piece, *king, pieces);
		}) == std::end(pieces);
	}
};

static auto getAllValidMovesForPiece(const Piece& piece, const std::vector<const Piece*>& pieces) noexcept {
	const auto& possibleMoves = piece.getAllPossibleMoves();
	std::vector<Position> validMoves;
	validMoves.reserve(100);

	for (const auto& destination : possibleMoves) {
		auto isValid = false;
		const auto& target = getPieceByPosition(destination, pieces);
		if (target == pieces.end())
			isValid = MoveRule::isMovePossible(piece, destination, pieces);
		else
			isValid = AttackRule::isAttackPossible(piece, **target, pieces);
		if (isValid)
			validMoves.push_back(destination);
	}
	return validMoves;
}

//TODO: REFACTOR THIS
static auto makeTestMove(const Piece& piece, const Position& position, const std::vector<const Piece*>& pieces) {
	auto copyPieces = pieces;

	const auto& toRemove_destination = std::find_if(copyPieces.begin(), copyPieces.end(), [&position](const Piece* p) { return position == p->getPosition(); });
	if (toRemove_destination != copyPieces.end())
		copyPieces.erase(toRemove_destination);
	
	const auto& toRemove_mover = std::find_if(copyPieces.begin(), copyPieces.end(), [&piece](const Piece* p) { return p->getPosition() == piece.getPosition(); });
	if (toRemove_mover != copyPieces.end())
		copyPieces.erase(toRemove_mover);

	//memoryleak so need to delete and pop this after
	if (isKing(piece))
		copyPieces.push_back(new King(position, piece.getOwner()));
	else
		copyPieces.push_back(new Pawn(position, piece.getOwner()));
	return copyPieces;
}

class CheckMateRule {
public:
	//TODO: REFACTOR THIS
	static bool isThereCheckMate(const Player& playerToVerify, const std::vector<const Piece*>& pieces) {
		for(auto& piece : pieces) {
			if(piece->getOwner() == playerToVerify) {
				const auto& movesToCheck = getAllValidMovesForPiece(*piece, pieces);
				std::cout << "piece:" << piece->toString() << std::endl;
				for(const auto& move : movesToCheck) {
					std::cout << "------> " << move.toString() << std::endl;
					auto copyPieces = makeTestMove(*piece, move, pieces);
					if (CheckRule::isThereNoCheck(playerToVerify, copyPieces)) {
						delete copyPieces.back();
						copyPieces.pop_back();
						return false;
					}
					delete copyPieces.back();
					copyPieces.pop_back();				
				}
			}
		}
		return true;
	}
};


