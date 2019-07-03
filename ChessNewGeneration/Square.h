#pragma once
#include <QPushButton>
#include <QPixmap>
#include "SingletonMove.h"

static std::string coordsToPosition(const unsigned column, const unsigned row) {
	if (column > 7 || row > 7)
		throw std::runtime_error("wrong cooridnates!");
	std::string position;
	position.push_back(static_cast<char>(static_cast<unsigned>('A') + column));
	position.push_back(static_cast<char>(static_cast<unsigned>('1') + row));
	return position;
}

class Square : public QPushButton {
	Q_OBJECT
	unsigned column;
	unsigned row;
	std::string position;
	void activate() {
		setStyleSheet("background-color: " + QColor(149, 137, 53).name());
	}
public:
	Square() : position("unknown") {}
	Square(const unsigned column, const unsigned row) : column(column), row(row) {
		position = coordsToPosition(column, row);
		setFixedSize(QSize(60, 60));
		setStyleSheet("border: none;");	
		deactivate();
		setAutoFillBackground(true);
		connect(this, SIGNAL(clicked()), this, SLOT(updateMove()));
		update();
	}

	std::string getPosition() {
		return position;
	}

	void setImage(const QIcon& icon) {
		setIcon(icon);
		setIconSize(QSize(50, 50));
		update();
	}

	void deactivate() {
		if ((column + row) % 2)
			setStyleSheet("background-color: " + QColor(245, 220, 175).name());
		else
			setStyleSheet("background-color: " + QColor(90, 50, 40).name());
	}
public slots:
	void updateMove() {
		activate();
		SingletonMove::getSingleton()->setPosition(position);
	}
};