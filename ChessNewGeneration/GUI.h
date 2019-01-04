#pragma once
#include "ChessGame.h"
#include <QGridLayout>
#include "Square.h"
#include <QPixmap>
#include <QPushButton>

class ChessGameGUI final : public QWidget {
	Q_OBJECT
	ChessGame chess_;
	std::vector<Square*> board_;
	void updateDisplay() noexcept {
		const auto pieces = chess_.getPiecesState();
		for (const auto& square : board_) {
			square->deactivate();
			square->removeImage();
		}	
		for (const auto& piece : pieces) {
			for (const auto& square : board_)
				if (square->getPosition() == piece->getPosition())
					square->setPieceImage(getImage(piece));
		}
	}
public:
	std::vector<Position> move_;

	ChessGameGUI(QWidget* parent = 0) : QWidget(parent) {
		auto chessBoard_layout = new QGridLayout;
		for (auto column = 0; column < 8; ++column) {
			for (auto row = 0; row < 8; ++row) {
				auto square = new Square(column, row, this);
				chessBoard_layout->addWidget(square, row, 7 - column);
				connect(square, SIGNAL(clicked()), this, SLOT(selectMove()));
				board_.push_back(square);
			}
		}
		chessBoard_layout->setHorizontalSpacing(0);
		chessBoard_layout->setVerticalSpacing(0);
		chessBoard_layout->setContentsMargins(0, 0, 0, 0);

		auto space_layout = new QVBoxLayout;
		space_layout->addItem(new QSpacerItem(10, 10));

		auto chessBoardAndSpace_layout = new QVBoxLayout;
		chessBoardAndSpace_layout->addLayout(space_layout);
		chessBoardAndSpace_layout->addLayout(chessBoard_layout);

		const auto undoMoveButton = new QPushButton("Backward");
		const auto resetButton = new QPushButton("Reset");
		connect(undoMoveButton, SIGNAL(clicked()), this, SLOT(undoMove()));
		connect(resetButton, SIGNAL(clicked()), this, SLOT(resetGame()));

		auto optionsButtons_layout = new QVBoxLayout;
		optionsButtons_layout->addWidget(undoMoveButton);
		optionsButtons_layout->addWidget(resetButton);

		auto all_layout = new QHBoxLayout;
		all_layout->addLayout(chessBoardAndSpace_layout);
		all_layout->addLayout(optionsButtons_layout);
		setLayout(all_layout);

		updateDisplay();
	}

public slots:
	void selectMove() {
		QObject* obj = sender();
		const auto square = qobject_cast<Square*>(sender());
		move_.push_back(square->getPosition());
		if (move_.size() == 1)
			square->activate();
		else if (move_.size() == 2) {
			chess_.nextMove({ move_[0], move_[1] });
			move_.clear();
			updateDisplay();
		}
	}

	void undoMove() {
		chess_.undoMove();
		updateDisplay();
	}

	void resetGame() {
		chess_.resetGame();
		updateDisplay();
	}

	QPixmap getImage(const std::unique_ptr<Piece>& ptr) const noexcept {
		if (dynamic_cast<Pawn*>(ptr.get())) {
			if(ptr->getOwner() == Player::White)
				return QPixmap("Images/pawn_white.svg");
			return QPixmap("Images/pawn_black.svg");
		}
		if (dynamic_cast<Knight*>(ptr.get())) {
			if (ptr->getOwner() == Player::White)
				return QPixmap("Images/knight_white.svg");
			return QPixmap("Images/knight_black.svg");
		}
		if (dynamic_cast<Bishop*>(ptr.get())) {
			if (ptr->getOwner() == Player::White)
				return QPixmap("Images/bishop_white.svg");
			return QPixmap("Images/bishop_black.svg");
		}
		if (dynamic_cast<Rook*>(ptr.get())) {
			if (ptr->getOwner() == Player::White)
				return QPixmap("Images/rook_white.svg");
			return QPixmap("Images/rook_black.svg");
		}
		if (dynamic_cast<Queen*>(ptr.get())) {
			if (ptr->getOwner() == Player::White)
				return QPixmap("Images/Queen_white.svg");
			return QPixmap("Images/Queen_black.svg");
		}
		if (dynamic_cast<King*>(ptr.get())) {
			if (ptr->getOwner() == Player::White)
				return QPixmap("Images/King_white.svg");
			return QPixmap("Images/King_black.svg");
		}
		throw std::exception();
	}
};
