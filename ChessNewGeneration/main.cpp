#include "ChessGame.h"
#include <QtCore/QCoreApplication>
int main(int  argc, char * argv[])
{
	QCoreApplication a(argc, argv);
	ChessGameConsole chess;
	chess.startGame();
	return QCoreApplication::exec();
}
