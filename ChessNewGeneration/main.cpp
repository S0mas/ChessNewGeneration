#include <QApplication>
#include "MainWindow.h"
#include <QMainWindow>

int main(int  argc, char * argv[])
{
	QApplication a(argc, argv);
	MainWindow c;
	c.show();
	return QApplication::exec();
}
