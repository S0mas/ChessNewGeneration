#pragma once

#include <QtWidgets/QMainWindow>
#include "ChessBoardGUI.h"

class MainWindow : public QMainWindow {
	std::unique_ptr<ChessboardGUI> cb;
	Q_OBJECT
public:
	MainWindow(QWidget *parent = Q_NULLPTR);
};
