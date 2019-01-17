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
	QLabel* winnerLabel;

	void updateWinnerLabel() const noexcept {
		if(chess_.isGameEnded())
			winnerLabel->setText("The winner is: " + chess_.getWinner());
		else
			winnerLabel->setText("Game in progress..");
	}

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
		updateWinnerLabel();
	}
public:
	std::vector<Position> move_;

	ChessGameGUI(QWidget* parent = 0) : QWidget(parent) {
		auto chessBoardLayout = new QGridLayout;
		for (auto column = 0; column < 8; ++column) {
			for (auto row = 0; row < 8; ++row) {
				auto square = new Square(column, row, this);
				chessBoardLayout->addWidget(square, row, 7 - column);
				connect(square, SIGNAL(clicked()), this, SLOT(selectMove()));
				board_.push_back(square);
			}
		}
		chessBoardLayout->setHorizontalSpacing(0);
		chessBoardLayout->setVerticalSpacing(0);
		chessBoardLayout->setContentsMargins(0, 0, 0, 0);

		const auto undoMoveButton = new QPushButton("Backward");
		undoMoveButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		const auto resetButton = new QPushButton("Reset");
		resetButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		winnerLabel = new QLabel("Game in progress..");
		winnerLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		connect(undoMoveButton, SIGNAL(clicked()), this, SLOT(undoMove()));
		connect(resetButton, SIGNAL(clicked()), this, SLOT(resetGame()));

		auto optionsButtonsLayout = new QVBoxLayout;
		optionsButtonsLayout->addWidget(winnerLabel);
		optionsButtonsLayout->addWidget(undoMoveButton);
		optionsButtonsLayout->addWidget(resetButton);

		auto spaceLayoutLeft = new QVBoxLayout;
		spaceLayoutLeft->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
		auto spaceLayoutRight = new QVBoxLayout;
		spaceLayoutRight->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

		auto chessboardAndSpaceAndOptionsLayout = new QHBoxLayout;
		chessboardAndSpaceAndOptionsLayout->addLayout(spaceLayoutLeft);
		chessboardAndSpaceAndOptionsLayout->addLayout(chessBoardLayout);
		chessboardAndSpaceAndOptionsLayout->addLayout(optionsButtonsLayout);
		chessboardAndSpaceAndOptionsLayout->addLayout(spaceLayoutRight);

		auto spaceLayoutTop = new QVBoxLayout;
		spaceLayoutTop->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
		auto spaceLayoutBottom = new QVBoxLayout;
		spaceLayoutBottom->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

		auto allLayouts = new QVBoxLayout;
		allLayouts->addLayout(spaceLayoutTop);
		allLayouts->addLayout(chessboardAndSpaceAndOptionsLayout);
		allLayouts->addLayout(spaceLayoutBottom);
		setLayout(allLayouts);

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
