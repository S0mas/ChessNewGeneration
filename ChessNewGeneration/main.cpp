#include <QApplication>
#include "GUI.h"
#include <QMainWindow>
int main(int  argc, char * argv[])
{
	QApplication a(argc, argv);
	ChessGameGUI c;
	QMainWindow mw;
	mw.show();
	mw.setCentralWidget(&c);
	return QApplication::exec();
}
