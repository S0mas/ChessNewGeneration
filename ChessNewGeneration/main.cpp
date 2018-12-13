#include "Position.h"
#include <QtCore/QCoreApplication>
int main(int  argc, char * argv[])
{
	QCoreApplication a(argc, argv);
	Position p("A2");
	return QCoreApplication::exec();
}
