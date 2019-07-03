#include "MainWindow.h"
#include "MockChessInterface.h"
#include "ChessGame.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent) {
	cb = std::make_unique<ChessboardGUI>(std::make_unique<ChessGame>());
	auto widget = new QWidget();
	widget->setLayout(cb->getLayout());
	setCentralWidget(widget);
}
