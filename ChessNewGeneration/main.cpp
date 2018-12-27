#include "ChessGame.h"
#include <QtCore/QCoreApplication>
int main(int  argc, char * argv[])
{
	QCoreApplication a(argc, argv);
	ChessGame chess;
	chess.startGame();
	return QCoreApplication::exec();
}
