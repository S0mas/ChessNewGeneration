#pragma once
#include "Rules.h"
#include "Chessboard.h"

class ChessGame {
public:
	ChessBoard chessboard_;
	Rules rules_;
	Player winner_;

	bool isGameEnded(const Player& playerToVerify, ChessBoard& chessboard) noexcept {
		return Rules::isThereCheckMate(playerToVerify, chessboard);
	}

	bool tryToMove(const Player& playerToMove, const SimpleMove& simpleMove, ChessBoard& chessboard) {
		if (!Rules::isLegalMove(playerToMove, simpleMove, chessboard))
			return false;

		chessboard.doMove(simpleMove);
	}

	SimpleMove requestMove() {

		return { Position("A1"), Position("A2") };
	}

	void startGame() {
		ChessBoard cb;
		Player activePlayer = Player::White;
		Player waitingPlayer = Player::Black;

		while (isGameEnded(activePlayer, cb)) {
			while (!tryToMove(activePlayer, requestMove(), cb)) {}
			std::swap(activePlayer, waitingPlayer);
		}

		winner_ = waitingPlayer;
	}
};