#pragma once
#include <string>
#include <vector>
#include <QObject>
#include <QString>

class ChessInterface : public QObject {
    Q_OBJECT
public:
    enum PieceType {
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
        BlackPawn,
        Empty
    };

    enum GameStatus {
        IN_PROGRESS,
        WHITE_WON,
        BLACK_WON,
        DRAW
    };

    enum PlayerColor {
        WHITE,
        BLACK
    };
    Q_ENUMS(PieceType)
    Q_ENUMS(GameStatus)
    Q_ENUMS(PlayerColor)
    Q_INVOKABLE virtual ~ChessInterface() {}
    Q_INVOKABLE virtual GameStatus getGameStatus() const = 0;
    Q_INVOKABLE virtual PieceType getPieceTypeByPosition(const QString& position) const noexcept = 0;
    Q_INVOKABLE virtual bool undoMove() = 0;
    Q_INVOKABLE virtual void move(const QString& originPosition, const QString& destinationPosition) = 0;
    Q_INVOKABLE virtual void promote(const PieceType& type) = 0;
    Q_INVOKABLE void reset() {
		while (undoMove()) {};
	}

signals:
    void moved();
    void promotion(PlayerColor player);
    void promoted();
};
