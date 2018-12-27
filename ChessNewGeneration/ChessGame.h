#pragma once
#include "Chessboard.h"
#include <iostream>

class ChessGame {
	
	FRIEND_TEST(ChessGameTests, checkmate);
	FRIEND_TEST(ChessGameTests, noCheckmate);
	FRIEND_TEST(ChessGameTests, check);
	FRIEND_TEST(ChessGameTests, noCheck);
	FRIEND_TEST(ChessGameTests, ownerIsActivePlayer);
	FRIEND_TEST(ChessGameTests, ownerIsNotActivePlayer);
	FRIEND_TEST(ChessGameTests, attackIsPossible);
	FRIEND_TEST(ChessGameTests, attackIsImpossible_sameOwner);
	FRIEND_TEST(ChessGameTests, attackIsImpossible_collision);
	FRIEND_TEST(ChessGameTests, attackIsImpossible_invalidDestinatino);
	FRIEND_TEST(ChessGameTests, moveIsPossible);
	FRIEND_TEST(ChessGameTests, moveIsImpossible_destinationOcupied);
	FRIEND_TEST(ChessGameTests, moveIsImpossible_collision);
	FRIEND_TEST(ChessGameTests, moveIsImpossible_invalidDestinatino);
	FRIEND_TEST(ChessGameTests, attackIsLegal);
	FRIEND_TEST(ChessGameTests, attackIsIllegal_kingIsTarget);
	FRIEND_TEST(ChessGameTests, attackIsIllegal_impossibleAttack);
	FRIEND_TEST(ChessGameTests, attackIsIllegal_isNotConsistentWithOtherRules);
	FRIEND_TEST(ChessGameTests, moveIsLegal);
	FRIEND_TEST(ChessGameTests, moveIsIllegal_impossibleMove);
	FRIEND_TEST(ChessGameTests, moveIsIllegal_isNotConsistentWithOtherRules);
	

	bool isAttackPossible(const Piece& attacker, const Piece& target) const noexcept {
		return attacker.getOwner() != target.getOwner() &&
			attacker.isConsistentWithAttackRules(target.getPosition()) &&
			!chessboard_.isThereCollision(attacker.getRoute(target.getPosition()));
	}

	bool isMovePossible(const Piece& mover, const Position& destination) const noexcept {
		return mover.isConsistentWithMoveRules(destination) &&
			!chessboard_.isPositionOccupied(destination) &&
			!chessboard_.isThereCollision(mover.getRoute(destination));
	}

	bool isThereCheck(const Player& playerToVerify) const noexcept {
		const auto& king = chessboard_.findKing(playerToVerify);
		const auto& pieces = chessboard_.getPieces();
		return pieces.cend() != std::find_if(pieces.cbegin(), pieces.cend(), [&king, this](const auto& piece) {
			return isAttackPossible(*piece, **king);
		});
	}

	bool isOwnerActivePlayer(const Piece& piece) const noexcept {
		return piece.getOwner() == activePlayer_;
	}

	//TODO: implement tests and other rules
	bool isConsistentWithOtherRules(const SimpleMove& move) noexcept {
		//is there check after move
		chessboard_.doMove(move);
		const auto& result = isThereCheck(activePlayer_);
		chessboard_.undoMove();
		return result;
	}

	bool isAttackLegal(const Piece& attacker, const Piece& target) noexcept {
		auto const& isLegal = !target.isKing() && isAttackPossible(attacker, target);
		return isLegal && isConsistentWithOtherRules({ target.getPosition(), target.getPosition() });
	}

	bool isMoveLegal(const Piece& pieceToMove, const Position& destination) noexcept {
		auto const& isLegal = isMovePossible(pieceToMove, destination);
		return isLegal && isConsistentWithOtherRules({ pieceToMove.getPosition(), destination });
	}

	//Todo: test
	bool checkIfMoveIsLegal(const SimpleMove& simpleMove) noexcept {
		const auto& pieceToMove = chessboard_.getPieceByPosition(simpleMove.origin_);
		if (pieceToMove != chessboard_.notFound() && isOwnerActivePlayer(**pieceToMove)) {
			const auto& pieceToAttack = chessboard_.getPieceByPosition(simpleMove.destination_);
			return pieceToAttack == chessboard_.notFound() ? isMoveLegal(**pieceToMove, simpleMove.destination_) : isAttackLegal(**pieceToMove, **pieceToAttack);
		}
		return false;
	}

	//Todo: test
	auto getAllValidMovesForPiece(const Piece& piece) const noexcept {
		const auto& possibleMoves = piece.getAllPossibleMoves();
		std::vector<Position> validMoves;
		validMoves.reserve(100);
		for (const auto& destination : possibleMoves) {
			auto isValid = false;
			const auto& target = chessboard_.getPieceByPosition(destination);
			if (target == chessboard_.notFound())
				isValid = isMovePossible(piece, destination);
			else
				isValid = isAttackPossible(piece, **target);
			if (isValid)
				validMoves.push_back(destination);
		}
		return validMoves;
	}

	bool isThereCheckmate() noexcept {
		for (const auto& piece : chessboard_.getPieces()) {
			if (piece->getOwner() == activePlayer_) {
				const auto& allPossibleDestinations = getAllValidMovesForPiece(*piece);
				return allPossibleDestinations.end() == std::find_if(allPossibleDestinations.begin(), allPossibleDestinations.end(), [this, origin = piece->getPosition()](const auto& destination) {
					return checkIfMoveIsLegal({ origin, destination }); });
			}
		}
		return true;
	}

	//Todo: test
	bool isGameEnded() noexcept {
		return isThereCheckmate();// && other stuff...;
	}

	SimpleMove requestMove() const noexcept {
		std::string origin;
		std::string destination;
		std::cin >> origin;
		std::cin >> destination;

		return { Position(origin), Position(destination) };
	}

	SimpleMove requestValidMove() noexcept {
		const auto& move = requestMove();
		return checkIfMoveIsLegal(move) ? move : requestValidMove();
	}
public:
	ChessBoard chessboard_;
	Player winner_;
	Player activePlayer_ = Player::White;

	void startGame() {
		Player waitingPlayer = Player::Black;

		while (!isGameEnded()) {
			chessboard_.doMove(requestValidMove());
			std::swap(activePlayer_, waitingPlayer);
		}

		winner_ = waitingPlayer;
	}
};
