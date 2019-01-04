#pragma once
#include "ChessGame.h"
#include <QGridLayout>
#include "Square.h"
#include <QPixmap>

class ChessGameGUI final : public QWidget {
	Q_OBJECT
	ChessGame chess_;
	std::vector<Square*> board_;
	void updateDisplay() noexcept {
		const auto pieces = chess_.getPiecesState();
		for (const auto& square : board_) {
			square->deactivate();
			square->clear();
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
		auto layout = new QGridLayout;
		for (auto column = 0; column < 8; ++column) {
			for (auto row = 0; row < 8; ++row) {
				auto square = new Square(column, row, this);
				layout->addWidget(square, row, 7 - column);
				connect(square, SIGNAL(clicked()), this, SLOT(selectMove()));
				board_.push_back(square);
			}
		}
		setLayout(layout);
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
