#pragma once
#include "Chessboard.h"
#include <iostream>

class ChessGame {
protected:
	ChessBoard chessboard_;
	Player winner_;
	Player activePlayer_ = Player::White;
	Player waitingPlayer_ = Player::Black;
	std::vector<SimpleMove> legalMoves_;
#ifdef Tests
	FRIEND_TEST(ChessGameTests, checkmate);
	FRIEND_TEST(ChessGameTests, noCheckmate_stalemate);
	FRIEND_TEST(ChessGameTests, noCheckmate_haveLegalMovesNoCheck);
	FRIEND_TEST(ChessGameTests, noCheckmate_haveLegalMoves);
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
	FRIEND_TEST(ChessGameTests, consistentWithOtherRules);
	FRIEND_TEST(ChessGameTests, notConsistentWithOtherRules);
	FRIEND_TEST(ChessGameTests, moveIsLegal_move);
	FRIEND_TEST(ChessGameTests, moveIsLegal_attack);
	FRIEND_TEST(ChessGameTests, moveIsIlegal_pieceNotFound);
	FRIEND_TEST(ChessGameTests, moveIsIlegal_ownerIsNotActivePlayer);
	FRIEND_TEST(ChessGameTests, moveIsIlegal_breaksMoveRules);
	FRIEND_TEST(ChessGameTests, moveIsIlegal_breaksAttackRules);
	FRIEND_TEST(ChessGameTests, gameEnded);
	FRIEND_TEST(ChessGameTests, gameNotEnded);
#endif	

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
		return pieces.end() != std::find_if(pieces.begin(), pieces.end(), [&king, this](const auto& piece) {
			return isAttackPossible(*piece, **king);
		});
	}

	bool isOwnerActivePlayer(const Piece& piece) const noexcept {
		return piece.getOwner() == activePlayer_;
	}

	bool isConsistentWithOtherRules(const SimpleMove& move) noexcept {
		//is there check after move
		chessboard_.doMove(move);
		const auto& result = !isThereCheck(activePlayer_);
		chessboard_.undoMove();
		return result;
	}

	bool isAttackLegal(const Piece& attacker, const Piece& target) noexcept {
		auto const& isLegal = !target.isKing() && isAttackPossible(attacker, target);
		return isLegal && isConsistentWithOtherRules({ attacker.getPosition(), target.getPosition() });
	}

	bool isMoveLegal(const Piece& pieceToMove, const Position& destination) noexcept {
		auto const& isLegal = isMovePossible(pieceToMove, destination);
		return isLegal && isConsistentWithOtherRules({ pieceToMove.getPosition(), destination });
	}

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

	auto getAllLegalMovesForPiece(const Piece& piece) noexcept {
		const auto& validMoves = getAllValidMovesForPiece(piece);
		const auto& origin = piece.getPosition();
		std::vector<SimpleMove> legalMoves;
		for (const auto& dest : validMoves) {
			if (checkIfMoveIsLegal({ origin, dest }))
				legalMoves.push_back({ origin, dest });
		}
		return legalMoves;
	}

	auto getAllLegalMoves(const Player& playerToMove) noexcept {
		legalMoves_.clear();
		for (const auto& piece : chessboard_.getPieces()) {
			if(piece->getOwner() == playerToMove)
				for (const auto& legalMove : getAllLegalMovesForPiece(*piece))
					legalMoves_.push_back(legalMove);
		}

		return legalMoves_;
	}

	bool isThereCheckmate() noexcept {
		return isThereCheck(activePlayer_) && getAllLegalMoves(activePlayer_).empty();
	}

	bool isThereStalemate() noexcept {
		return !isThereCheck(activePlayer_) && getAllLegalMoves(activePlayer_).empty();
	}

	bool isGameEnded() noexcept {
		return isThereCheckmate() || isThereStalemate();// && other stuff...;
	}

	SimpleMove requestValidMove() noexcept {
		const auto& move = requestMove();
		return checkIfMoveIsLegal(move) ? move : requestValidMove();
	}

	virtual SimpleMove requestMove() const noexcept { throw std::exception(); };
public:
	virtual ~ChessGame() = default;
	void startGame() {
		while (!isGameEnded()) {
			chessboard_.doMove(requestValidMove());
			std::swap(activePlayer_, waitingPlayer_);
		}

		winner_ = waitingPlayer_;
	}

	auto getPiecesState() const noexcept  {
		std::vector<std::unique_ptr<Piece>> piecesCopy;
		for(const auto& piece : chessboard_.getPieces())
			piecesCopy.push_back(piece->clone());
		
		return piecesCopy;
	}
};

//example use
class ChessGameConsole final : public  ChessGame {
	SimpleMove requestMove() const noexcept final {
		std::string origin;
		std::string destination;
		std::cin >> origin;
		std::cin >> destination;
		return { Position(origin),  Position(destination) };
	}
};

