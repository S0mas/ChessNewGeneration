#include <QtCore/QCoreApplication>
#include "GUI.h"
#include <QMainWindow>
int main(int  argc, char * argv[])
{
	QCoreApplication a(argc, argv);
	ChessGameGUI c;
	QMainWindow mw;
	mw.setCentralWidget(&c);
	return QCoreApplication::exec();
}
