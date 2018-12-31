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

	bool isBlack() const noexcept {
		return (position.column_ + position.row_) % 2 == 0;
	}
public:
	Square(const int column, const int row, QWidget *parent = 0) : QLabel(parent), position(column, row) {
		if(isBlack()) {
			this->setStyleSheet("QLabel {background-color: rgb(120, 120, 90);}:hover{background-color: rgb(170,85,127);}");
		}
		else {
			this->setStyleSheet("QLabel {background-color: rgb(211, 211, 158);}:hover{background-color: rgb(170,95,127);}");
		}
	}

	void setPieceImage(const QPixmap& image) noexcept {
		this->setPixmap(image);
	}

	Position getPosition() const noexcept {
		return position;
	}

signals:
	void clicked();

protected:
	void mousePressEvent(QMouseEvent* event) {
		emit clicked();
	}
};
