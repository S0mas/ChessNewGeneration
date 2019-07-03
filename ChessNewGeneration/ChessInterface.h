#pragma once
#include <string>
#include <vector>

enum class PieceType {
	WhiteKing,
	WhiteQueen,
	WhiteRook,
	WhiteBishop,
	WhiteKnight,
	WhitePawn,
	BlackKing,
	BlackQueen,
	BlackRook,
	BlackBishop,
	BlackKnight,
	BlackPawn
};

enum class GameStatus {
	IN_PROGRESS,
	WHITE_WON,
	BLACK_WON,
	DRAW
};

using BoardState = std::vector<std::pair<PieceType, std::string>>;

class ChessInterface {
public:
	virtual GameStatus getGameStatus() const = 0;
	virtual BoardState getBoardState() const = 0;
	virtual bool undoMove() = 0;
	virtual bool move(const std::string& originPosition, const std::string& destinationPosition) = 0;
	void reset() {
		while (undoMove()) {};
	}
};