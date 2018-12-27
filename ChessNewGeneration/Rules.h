#pragma once
#include <vector>
#include <algorithm>
#include "Chessboard.h"

class Rules {

	FRIEND_TEST(CheckTests, noCheck);
	FRIEND_TEST(CheckTests, check);
	FRIEND_TEST(CollisionTests, noCollision);
	FRIEND_TEST(CollisionTests, collision);
	FRIEND_TEST(AttackTests, legalAttack);
	FRIEND_TEST(AttackTests, notLegalAttack_cantAttackKing);
	FRIEND_TEST(AttackTests, notLegalAttack_impossible);
	FRIEND_TEST(AttackTests, possibleAttack);
	FRIEND_TEST(AttackTests, impossibleAttack_invalidPath);
	FRIEND_TEST(AttackTests, impossibleAttack_KingAsTarget);
	FRIEND_TEST(AttackTests, impossibleAttack_collision);
	FRIEND_TEST(AttackTests, impossibleAttack_sameOwner);

	static bool isOwnerActivePlayer(const Piece& piece, const Player& activePlayer) noexcept {
		return piece.getOwner() == activePlayer;
	}

	static bool isAttackLegal(const Piece& attacker, const Piece& target, const ChessBoard& chessboard) {
		return !target.isKing() && isAttackPossible(attacker, target, chessboard);
	}

	static bool isAttackPossible(const Piece& attacker, const Piece& target, const ChessBoard& chessboard) {
		return attacker.getOwner() != target.getOwner() && attacker.isConsistentWithAttackRules(target.getPosition()) && !chessboard.isThereCollision(attacker.getRoute(target.getPosition()));
	}

	static bool isMovePossible(const Piece& mover, const Position& destination, const ChessBoard& chessboard) {
		return mover.isConsistentWithMoveRules(destination) && !chessboard.isPositionOccupied(destination) && !chessboard.isThereCollision(mover.getRoute(destination));
	}

	static bool isThereCheck(const Player& playerToVerify, const ChessBoard& chessboard) {
		const auto& king = chessboard.findKing(playerToVerify);
		const auto& pieces = chessboard.getPieces();
		return std::find_if(pieces.cbegin(), pieces.cend(), [&king, &chessboard](const auto& piece) {
			return isAttackPossible(*piece, **king, chessboard);
		}) == pieces.cend();
	}

	static auto getAllValidMovesForPiece(const Piece& piece, const ChessBoard& chessboard) noexcept {
		const auto& possibleMoves = piece.getAllPossibleMoves();
		std::vector<Position> validMoves;
		validMoves.reserve(100);
		for (const auto& destination : possibleMoves) {
			auto isValid = false;
			const auto& target = chessboard.getPieceByPosition(destination);
			if (target == chessboard.getPieces().cend())
				isValid = isMovePossible(piece, destination, chessboard);
			else
				isValid = isAttackPossible(piece, **target, chessboard);
			if (isValid)
				validMoves.push_back(destination);
		}
		return validMoves;
	}
public:
	static bool isLegalMove(const Player& playerToMove, const SimpleMove& simpleMove, ChessBoard& chessboard) noexcept {
		auto const& pieceToMove = chessboard.getPieceByPosition(simpleMove.origin_);
		if (pieceToMove == chessboard.notFound() || isOwnerActivePlayer(**pieceToMove, playerToMove))
			return false;
		auto const& pieceToAttack = chessboard.getPieceByPosition(simpleMove.destination_);

		auto const& isLegal = (pieceToAttack == chessboard.notFound()) ? isMovePossible(**pieceToMove, simpleMove.destination_, chessboard)
			: isAttackLegal(**pieceToMove, **pieceToAttack, chessboard);
		
		auto result = false;
		if (isLegal) {
			chessboard.doMove(simpleMove);
			result = isThereCheck(playerToMove, chessboard);
			chessboard.undoMove();
		}
		return result;
	}

	static bool isThereCheckMate(const Player& playerToVerify, ChessBoard& chessboard) {
		for (const auto& piece : chessboard.getPieces()) {
			if (piece->getOwner() == playerToVerify) {
				const auto& allPossibleDestinations = getAllValidMovesForPiece(*piece, chessboard);
				return allPossibleDestinations.end() == std::find_if(allPossibleDestinations.begin(), allPossibleDestinations.end(), [&playerToVerify, &chessboard, origin = piece->getPosition()](const auto& destination) {
					return isLegalMove(playerToVerify, { origin, destination }, chessboard); });
			}
		}
		return true;
	}
};

