#pragma once
#include "Chessboard.h"
#include <string>
#include <iostream>

class ChessGame : public ChessInterface {
protected:
	ChessBoard chessboard;
	Player activePlayer_;
	Player waitingPlayer_;
	std::vector<Move> legalMoves_;
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
			!chessboard.isThereCollision(attacker.getRoute(target.getPosition()));
	}

	bool isMovePossible(const Piece& mover, const Position& destination) const noexcept {
		return mover.isConsistentWithMoveRules(destination) &&
			!chessboard.isPositionOccupied(destination) &&
			!chessboard.isThereCollision(mover.getRoute(destination));
	}

	bool isThereCheckAfterMove(const Move& move) {
		chessboard.doMove(move);
		const auto& result = isThereCheck(activePlayer_);
		chessboard.undoMove();
		return result;
	}

	bool isThereCheck(const Player& playerToVerify) const noexcept {
		const auto& king = chessboard.findKing(playerToVerify);
		const auto& pieces = chessboard.getPieces();
		return pieces.end() != std::find_if(pieces.begin(), pieces.end(), [&king, this](const auto& piece) {
			return piece->isAlive() && isAttackPossible(*piece, **king);
		});
	}

	bool isOwnerActivePlayer(const Piece& piece) const noexcept {
		return piece.getOwner() == activePlayer_;
	}

	void calculateAllLegalMoves() noexcept {
		legalMoves_.clear();
		for (const auto& simpleMove : chessboard.getAllPossibleMoves()) {
			auto move = isValidMove(simpleMove);
			if (move.type != Move::MoveType::Invalid && !isThereCheckAfterMove(move))
				legalMoves_.push_back(move);			
		}
	}

	bool isThereCheckmate() const noexcept {
		return isThereCheck(activePlayer_) && legalMoves_.empty();
	}

	bool isThereStalemate() const noexcept {
		return !isThereCheck(activePlayer_) && legalMoves_.empty();
	}

	bool isThereDraw() const noexcept {
		return !isThereCheck(activePlayer_) && legalMoves_.empty();
	}

	Move isValidMove(const SimpleMove& sm) noexcept {
		auto origin = chessboard.getPieceByPosition(sm.origin_);
		if (origin == chessboard.notFound() || !isOwnerActivePlayer(**origin))
			return Move::invalid();
		auto target = chessboard.getPieceByPosition(sm.destination_);
		auto targetPiece = (target == chessboard.notFound()) ? nullptr : target->get();
		if (targetPiece && targetPiece->isKing())
			return Move::invalid();
		if (isValidPromotionMove(**origin, targetPiece, sm.destination_))
			return Move(origin->get(), sm, targetPiece, Move::MoveType::Promotion);
		else if (isValidDefaultMove(**origin, targetPiece, sm.destination_))
			return Move(origin->get(), sm, targetPiece);
		else if (isValidCastlingMove(**origin, targetPiece, sm.destination_))
			return Move(origin->get(), sm, nullptr, Move::MoveType::Castle, findCastlingTower(sm.destination_));
		else if (isValidEnPasantMove(**origin, targetPiece, sm.destination_))
			return Move(origin->get(), sm, findEnPassantKilledPawn(sm.destination_), Move::MoveType::EnPassant);
		return Move::invalid();
	}

	bool isValidPromotionMove(const Piece& subject, const Piece* target, const Position& destination) const {
		return isValidDefaultMove(subject, target, destination) && subject.isPawn() && destination.isDestinationPromotionRow();
	}

	bool isValidCastlingMove(const Piece& subject, const Piece* target, const Position& destination) {
		return subject.isKing() && subject.hasNotMoved() && subject.getPosition().row_ == destination.row_ && destination.isCastlingColumn() && !chessboard.isPositionOccupied(destination)
			&& hasCastlingTowerNotMoved(destination) && !chessboard.isThereCollision(subject.getPosition().getSimplestRoute(destination)) && !isThereCheckAfterMove(isValidMove(SimpleMove(subject.getPosition(), subject.getPosition().getSimplestRoute(destination)[0])));
	}

	bool isValidEnPasantMove(const Piece& subject, const Piece* target, const Position& destination) const {
		return subject.isPawn() && isEnpassantRow(destination.row_) && subject.isConsistentWithAttackRules(destination) && checkIfLastMoveWasTheLongJumpOfSpecificColumnPawn(destination);
	}

	bool isValidDefaultMove(const Piece& subject, const Piece* target, const Position& destination) const {
		return isMovePossible(subject, destination) || target && isAttackPossible(subject, *target);
	}

	bool checkIfLastMoveWasTheLongJumpOfSpecificColumnPawn(const Position& destination) const {
		if (chessboard.wasThereAnyMove()) {
			const auto& lastMove = chessboard.getLastMove();
			const auto& wasLongJump = abs(lastMove.getSimpleMove().origin_.row_ - lastMove.getSimpleMove().destination_.row_) == 2;
			return wasLongJump && lastMove.getSubject()->isPawn() && lastMove.getSimpleMove().origin_.column_ == destination.column_;
		}
		return false;
	}

	Rook* findCastlingTower(const Position& castlingDestination) const {
		Position rookPosition("A2");
		if (castlingDestination.column_ == 2)
			rookPosition = (castlingDestination.row_ == 7) ? Position("A8") : Position("A1");
		else
			rookPosition = (castlingDestination.row_ == 7) ? Position("H8") : Position("H1");
		auto rook = chessboard.getPieceByPosition(rookPosition);
		return (rook != chessboard.notFound() && dynamic_cast<Rook*>(rook->get())) ? dynamic_cast<Rook*>(rook->get()) : nullptr;
	}

	bool hasCastlingTowerNotMoved(const Position& castlingDestination) const {
		const auto& tower = findCastlingTower(castlingDestination);
		return tower ? tower->hasNotMoved() : false;
	}


	bool isEnpassantRow(const int row) const noexcept {
		return activePlayer_ == Player::White ? row == 5 : row == 2;
	}

	Pawn* findEnPassantKilledPawn(const Position& enPassantDestination) const {
		Position killedPawnPosition(enPassantDestination.column_, enPassantDestination.row_ == 2 ? 3 : 4);
		auto pawn = chessboard.getPieceByPosition(killedPawnPosition);
		return dynamic_cast<Pawn*>(pawn->get());
	}

public:
	ChessGame() noexcept {
		activePlayer_ = Player::White;
		waitingPlayer_ = Player::Black;
	}
    ~ChessGame() override = default;

	void resetGame() {
		while (chessboard.wasThereAnyMove())
			undoMove();
	}

	GameStatus getGameStatus() const override {
		return GameStatus::IN_PROGRESS;
	}

	bool undoMove() override {
		if (chessboard.wasThereAnyMove()) {
			chessboard.undoMove();
			std::swap(activePlayer_, waitingPlayer_);
			return true;
		}
		else
			return false;
	}

    bool move(const QString& originPosition, const QString& destinationPosition) override {
        auto nextMove = SimpleMove(Position(originPosition.toStdString()), Position(destinationPosition.toStdString()));
		if(!isGameEnded()) {
			const auto& move = std::find(legalMoves_.begin(), legalMoves_.end(), nextMove);
			if (move != legalMoves_.end()) {
				chessboard.doMove(*move);
                if((*move).type == Move::Promotion)
                    emit promotion();
				std::swap(activePlayer_, waitingPlayer_);
				return true;
			}
		}
		return false;
	}

    bool promote(const PieceType& type) override {
        if(chessboard.wasThereAnyMove()) {
            auto lastMove = chessboard.getLastMove();
            if(lastMove.type == Move::Promotion && lastMove.getSubject()->isPawn()){
                auto piece = chessboard.getPieceByPosition(lastMove.getSimpleMove().destination_);
                piece->reset(dynamic_cast<Pawn*>(piece->get())->promote(type).release());
                return true;
            }
        }
        return false;
    }

    PieceType getPieceTypeByPosition(const QString& position) const noexcept override {
        auto piece = chessboard.getPieceByPosition(Position(position.toStdString()));
        if(piece == chessboard.notFound())
            return Empty;
        return (*piece)->getType();
	}

    QString getWinner() const noexcept {
		if (isThereCheckmate())
            return waitingPlayer_ == Player::White ? "white" : "black";
		else if (isThereStalemate())
            return "draw";
        return "game in progress";
	}

	bool isGameEnded() noexcept {
		calculateAllLegalMoves();
		return isThereCheckmate() || isThereStalemate();// && other stuff...;
	}
};
