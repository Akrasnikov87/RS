#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	MainWindow RS232;
	RS232.show();

	return a.exec();
}
