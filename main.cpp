#include <QApplication>

#include "ui/mainwindow.h"

int main(int argc, char* argv[]) {

	QApplication a(argc, argv);

	QCoreApplication::setApplicationName("Octree Modeling");
	QCoreApplication::setApplicationVersion("1.0.0");

	MainWindow w;

	w.show();

	return a.exec();
}