#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtSerialPort>
#include <stdio.h>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_pushButton_2_clicked(bool checked);
    void setDataBits(int);
    void setParity(int);
    void setStopBits(int);
	void send();
	void receive();

private:
	Ui::MainWindow *ui;
	QSerialPort *port = new QSerialPort(this);
	QScrollBar *scrollBar;
    QByteArray byteArray;
};
#endif //MAINWINDOW_H
