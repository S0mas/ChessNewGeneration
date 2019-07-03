#pragma once
#include "Square.h"
#include <array>
#include <QPointer>
#include <QGridLayout>
#include "ChessInterface.h"
#include <memory>

class ChessboardGUI : public QWidget {
	Q_OBJECT
	const static unsigned rowNo = 8;
	const static unsigned columnNo = 8;
	std::array<QPointer<Square>, columnNo*rowNo> squares;
	QPointer<QLayout> layout;
	std::unique_ptr<ChessInterface> chessEngine;

	QIcon loadImage(const PieceType piece) {
		if(piece == PieceType::WhiteKing)
			return QIcon(":Images/king_white.svg");
		if (piece == PieceType::WhiteQueen)
			return QIcon(":Images/queen_white.svg");
		if (piece == PieceType::WhiteRook)
			return QIcon(":Images/rook_white.svg");
		if (piece == PieceType::WhiteBishop)
			return QIcon(":Images/bishop_white.svg");
		if (piece == PieceType::WhiteKnight)
			return QIcon(":Images/knight_white.svg");
		if (piece == PieceType::WhitePawn)
			return QIcon(":Images/pawn_white.svg");
		if (piece == PieceType::BlackKing)
			return QIcon(":Images/king_black.svg");
		if (piece == PieceType::BlackQueen)
			return QIcon(":Images/queen_black.svg");
		if (piece == PieceType::BlackRook)
			return QIcon(":Images/rook_black.svg");
		if (piece == PieceType::BlackBishop)
			return QIcon(":Images/bishop_black.svg");
		if (piece == PieceType::BlackKnight)
			return QIcon(":Images/knight_black.svg");
		if (piece == PieceType::BlackPawn)
			return QIcon(":Images/pawn_black.svg");
		return QIcon();
	}
public:
	ChessboardGUI(std::unique_ptr<ChessInterface>&& chessEng) : chessEngine(std::move(chessEng))  {
		for (auto column = 0u; column < columnNo; ++column) {
			for (auto row = 0u; row < rowNo; ++row) {
				const auto& index = column * columnNo + row;
				squares[index] = new Square(column, row);
			}
		}
		auto gLayout = new QGridLayout();
		for (auto column = 0u; column < columnNo; ++column) {
			for (auto row = 0u; row < rowNo; ++row) {
				const auto& index = column * columnNo + row;
				gLayout->addWidget(squares[index], row, column);
			}
		}
		gLayout->setSpacing(0);
		connect(SingletonMove::getSingleton(), SIGNAL(moveIsReady()), this, SLOT(sendMove()));
		setLayout(gLayout);
		update();
	}

	void clear() {
		for (auto& square : squares) {
			square->deactivate();
			square->setImage(QIcon());
		}
	}

	void setResult(const GameStatus status) {

	}

	void update() {
		clear();
		auto boardState = chessEngine->getBoardState();
		for (auto& square : squares) {
			for (auto const& pieceState : boardState) {
				if (square->getPosition() == pieceState.second) {
					square->setImage(loadImage(pieceState.first));
					break;
				}
			}
		}		
		setResult(chessEngine->getGameStatus());
	}
public slots:
	void sendMove() {
		chessEngine->move(SingletonMove::getSingleton()->originPosition, SingletonMove::getSingleton()->destinationPosition);
		update();
	}
};