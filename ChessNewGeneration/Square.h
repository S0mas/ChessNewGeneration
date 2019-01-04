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
	QPixmap image;
	bool isBlack() const noexcept {
		return (position.column_ + position.row_) % 2 == 0;
	}
public:
	Square(const int column, const int row, QWidget *parent = 0) : QLabel(parent), position(column, row) {
		if(isBlack())
			background = "QLabel {background-color: rgb(47,79,79);}:hover{background-color: rgb(170,85,127);}";
		else 
			background = "QLabel {background-color: rgb(211,211,200);}:hover{background-color: rgb(170,95,127);}";
		this->setMinimumSize(QSize(64, 64));
		this->setStyleSheet(background);
		QSizePolicy p = this->sizePolicy();
		p.setHeightForWidth(true);
		p.setWidthForHeight(true);
		this->setSizePolicy(p);
	}

	void resizeEvent(QResizeEvent* event) final {
		if(!image.isNull())
			this->setPixmap(image.scaled(this->width(), this->height(), Qt::KeepAspectRatio));
	}

	void setPieceImage(const QPixmap& pixmap) noexcept {
		image = pixmap;
		this->setPixmap(image.scaled(this->width(), this->height(), Qt::KeepAspectRatio));
	}

	void removeImage() noexcept {
		image = QPixmap();
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
	void mousePressEvent(QMouseEvent* event) final {
		emit clicked();
	}
};
