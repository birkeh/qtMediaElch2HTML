#include "cmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName("WIN-DESIGN");
	QCoreApplication::setOrganizationDomain("windesign.at");
	QCoreApplication::setApplicationName("qtMediaElch2HTML");

	QApplication a(argc, argv);
	cMainWindow w;
	w.showMaximized();

	return a.exec();
}
