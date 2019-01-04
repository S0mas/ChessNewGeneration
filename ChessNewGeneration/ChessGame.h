#pragma once
#include "Chessboard.h"
#include <QString>

class ChessGame {
protected:
	ChessBoard chessboard_;
	Player activePlayer_;
	Player waitingPlayer_;
	mutable std::vector<SimpleMove> legalMoves_;
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
	FRIEND_TEST(ChessGameTests, gameEnded_Checkmate);
	FRIEND_TEST(ChessGameTests, gameEnded_Stalemate);
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

	bool isThereCheck(const Player& playerToVerify, const ChessBoard& chessboard) const noexcept {
		const auto& king = chessboard.findKing(playerToVerify);
		const auto& pieces = chessboard.getPieces();
		return pieces.end() != std::find_if(pieces.begin(), pieces.end(), [&king, this](const auto& piece) {
			return isAttackPossible(*piece, **king);
		});
	}

	bool isOwnerActivePlayer(const Piece& piece) const noexcept {
		return piece.getOwner() == activePlayer_;
	}

	bool isConsistentWithOtherRules(const SimpleMove& move, ChessBoard& chessBoardCopy) const noexcept {
		//is there check after move
		chessBoardCopy.doMove(move);
		const auto& result = !isThereCheck(activePlayer_, chessBoardCopy);
		chessBoardCopy.undoMove();
		return result;
	}

	bool isAttackLegal(const Piece& attacker, const Piece& target, ChessBoard& chessBoardCopy) const noexcept {
		auto const& isLegal = !target.isKing() && isAttackPossible(attacker, target);
		return isLegal && isConsistentWithOtherRules({ attacker.getPosition(), target.getPosition() }, chessBoardCopy);
	}

	bool isMoveLegal(const Piece& pieceToMove, const Position& destination, ChessBoard& chessBoardCopy) const noexcept {
		auto const& isLegal = isMovePossible(pieceToMove, destination);
		return isLegal && isConsistentWithOtherRules({ pieceToMove.getPosition(), destination }, chessBoardCopy);
	}

	bool checkIfMoveIsLegal(const SimpleMove& simpleMove, ChessBoard& chessBoardCopy) const noexcept {
		const auto& pieceToMove = chessboard_.getPieceByPosition(simpleMove.origin_);
		if (pieceToMove != chessboard_.notFound() && isOwnerActivePlayer(**pieceToMove)) {
			const auto& pieceToAttack = chessboard_.getPieceByPosition(simpleMove.destination_);
			return pieceToAttack == chessboard_.notFound() ? isMoveLegal(**pieceToMove, simpleMove.destination_, chessBoardCopy) : isAttackLegal(**pieceToMove, **pieceToAttack, chessBoardCopy);
		}
		return false;
	}

	auto getAllLegalMovesForPiece(const Piece& piece, ChessBoard& chessBoardCopy) const noexcept {
		const auto& possibleDestination = piece.getAllPossibleMoves();
		const auto& origin = piece.getPosition();
		std::vector<SimpleMove> legalMoves;
		for (const auto& dest : possibleDestination) {
			if (origin == dest) continue;
			SimpleMove move = { origin, dest };
			if (checkIfMoveIsLegal(move, chessBoardCopy))
				legalMoves.push_back(std::move(move));
		}
		return legalMoves;
	}

	void calculateAllLegalMoves(const Player& playerToMove) const noexcept {
		legalMoves_.clear();
		auto chessboardCopy(chessboard_);
		for (const auto& piece : chessboardCopy.getPieces()) {
			if(piece->getOwner() == playerToMove)
				for (auto&& legalMove : getAllLegalMovesForPiece(*piece, chessboardCopy))
					legalMoves_.push_back(std::move(legalMove));
		}
	}

	bool isThereCheckmate() const noexcept {
		return isThereCheck(activePlayer_, chessboard_) && legalMoves_.empty();
	}

	bool isThereStalemate() const noexcept {
		return !isThereCheck(activePlayer_, chessboard_) && legalMoves_.empty();
	}

	bool isThereDraw() const noexcept {
		return !isThereCheck(activePlayer_, chessboard_) && legalMoves_.empty();
	}

	bool isValidMove(const SimpleMove& nextMove) const noexcept {
		return std::find(legalMoves_.begin(), legalMoves_.end(), nextMove) != legalMoves_.end();
	}
public:
	ChessGame() noexcept {
		activePlayer_ = Player::White;
		waitingPlayer_ = Player::Black;
	}
	virtual ~ChessGame() = default;

	void resetGame() {
		chessboard_.reset();
	}

	QString getWinner() const noexcept {
		QString result = "game in progress";
		
		if (isThereCheckmate())
			result = waitingPlayer_ == Player::White ? "white" : "black";
		else if (isThereStalemate())
			result = "draw";
		return result;
	}

	void nextMove(const SimpleMove& nextMove) noexcept {
		if(!isGameEnded() && isValidMove(nextMove)) {
			chessboard_.doMove(nextMove);
			std::swap(activePlayer_, waitingPlayer_);
		}
	}

	void undoMove() noexcept {
		if(chessboard_.undoMove())
			std::swap(activePlayer_, waitingPlayer_);
	}

	auto getPiecesState() const noexcept  {
		std::vector<std::unique_ptr<Piece>> piecesCopy;
		for(const auto& piece : chessboard_.getPieces())
			piecesCopy.push_back(piece->clone());
		
		return piecesCopy;
	}

	bool isGameEnded() const noexcept {
		calculateAllLegalMoves(activePlayer_);
		return isThereCheckmate() || isThereStalemate();// && other stuff...;
	}
};

