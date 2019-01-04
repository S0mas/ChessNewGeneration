#pragma once
#include <string>
#include <QLabel>
#include "Position.h"
#include <QWidget>
#include <QLabel>
#include <QGraphicsPixmapItem>

class Square : public QLabel {
	Q_OBJECT
	Position position;
	QString background;
	bool isBlack() const noexcept {
		return (position.column_ + position.row_) % 2 == 0;
	}
public:
	Square(const int column, const int row, QWidget *parent = 0) : QLabel(parent), position(column, row) {
		if(isBlack())
			background = "QLabel {background-color: rgb(120, 120, 90);}:hover{background-color: rgb(170,85,127);}";
		else 
			background = "QLabel {background-color: rgb(211, 211, 158);}:hover{background-color: rgb(170,95,127);}";

		this->setStyleSheet(background);
	}

	void setPieceImage(const QPixmap& image) noexcept {
		this->setPixmap(image);
	}

	Position getPosition() const noexcept {
		return position;
	}

	void activate() noexcept {
		this->setStyleSheet("QLabel {background-color: rgb(249, 166, 2);}:hover{background-color: rgb(249,166,2);}");
	}

	void deactivate() noexcept {
		this->setStyleSheet(background);
	}

signals:
	void clicked();

protected:
	void mousePressEvent(QMouseEvent* event) {
		emit clicked();
	}
};
