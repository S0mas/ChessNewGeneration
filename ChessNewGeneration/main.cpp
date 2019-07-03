#include <QApplication>
#include <QMainWindow>
#include "ChessBoardGUI.h"
#include "ChessGame.h"

int main(int  argc, char * argv[])
{
	QApplication a(argc, argv);
	QMainWindow mw;
	ChessboardGUI chess(std::make_unique<ChessGame>());
	mw.setCentralWidget(&chess);
	mw.show();
	return QApplication::exec();
}
