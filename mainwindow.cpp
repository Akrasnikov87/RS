#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
    setWindowTitle("COM");

    foreach (const QSerialPortInfo &serialPortInfo,QSerialPortInfo::availablePorts())
        ui->comboBox->addItem(serialPortInfo.portName());

    connect (ui->pushButton,&QPushButton::clicked,this,&MainWindow::send);
    connect (port,&QSerialPort::readyRead,this,&MainWindow::receive);
    connect (ui->comboBox_3,SIGNAL(currentIndexChanged(int)),SLOT(setDataBits(int)));
    connect (ui->comboBox_4,SIGNAL(currentIndexChanged(int)),SLOT(setParity(int)));
    connect (ui->comboBox_5,SIGNAL(currentIndexChanged(int)),SLOT(setStopBits(int)));
    connect (ui->pushButton_3,&QPushButton::clicked,[&](){ ui->textEdit->clear(); });
	ui->radioButton->click();
	scrollBar = ui->textEdit->verticalScrollBar();
}
MainWindow::~MainWindow() { delete ui; }
void MainWindow::send() {
    if (ui->radioButton->isChecked()) port->write(ui->lineEdit->text().toLocal8Bit().append('\n'));
    if (ui->radioButton_2->isChecked()) port->write(ui->lineEdit->text().toLocal8Bit().append('\r'));
    if (ui->radioButton_3->isChecked()) port->write(ui->lineEdit->text().toLocal8Bit().append('\n').append('\r'));
    if (ui->radioButton_4->isChecked()) port->write(ui->lineEdit->text().toLocal8Bit().append('\r').append('\n'));
    if (ui->radioButton_5->isChecked()) port->write(QByteArray::fromHex(ui->lineEdit->text().toLocal8Bit()));
    port->waitForBytesWritten(1000);
}
void MainWindow::receive() {
    byteArray.clear();
    byteArray = port->readAll();
    if (ui->radioButton_5->isChecked()) {
        static const char numbers[] = "0123456789ABCDEF";
        QString string;
        string.resize(byteArray.size()*3-1);
        int rpos = 0;
        for (int i = 0; i < byteArray.size(); ++i) {
            char c = byteArray.at(i);
            string[rpos++] = QChar(numbers[(c >> 4) % 16]);
            string[rpos++] = QChar(numbers[c % 16]);
            if (i != byteArray.size() - 1)
                string[rpos++] = QChar(' ');
        }
        ui->textEdit->setText(ui->textEdit->toPlainText() + string.append('\n'));
    } else {
        ui->textEdit->setText(ui->textEdit->toPlainText() + byteArray);
    }
	scrollBar->setValue(scrollBar->maximum());
}
void MainWindow::on_pushButton_2_clicked(bool checked) {
    if (checked) {
		port->setPortName(ui->comboBox->currentText());
		port->setBaudRate(ui->comboBox_2->currentText().toInt());
		port->setFlowControl(QSerialPort::NoFlowControl);
        if (!port->open(QIODevice::ReadWrite)) {
            ui->pushButton_2->setChecked(false);
            QMessageBox::critical(this,"Error","port is not open");
        } else {
            ui->comboBox->setEnabled(false);
            ui->comboBox_2->setEnabled(false);
            ui->pushButton_2->setText("Disconnect");
            ui->comboBox_5->setEnabled(false);
        }
    } else {
        ui->comboBox->setEnabled(true);
		ui->comboBox_2->setEnabled(true);
		ui->comboBox_5->setEnabled(true);
        ui->pushButton_2->setText("Connect");
        if (port->isOpen())
            port->close();
	}
}
void MainWindow::setDataBits(int index) {
    switch(index) {
	case 0:
		port->setDataBits(QSerialPort::Data8);
		break;
	case 1:
		port->setDataBits(QSerialPort::Data7);
		break;
	case 2:
		port->setDataBits(QSerialPort::Data6);
		break;
	case 3:
		port->setDataBits(QSerialPort::Data5);
		break;
	}
}
void MainWindow::setParity(int index) {
    switch(index) {
	case 0:
		port->setParity(QSerialPort::NoParity);
		break;
	case 1:
		port->setParity(QSerialPort::EvenParity);
		break;
	case 2:
		port->setParity(QSerialPort::OddParity);
		break;
	case 3:
		port->setParity(QSerialPort::SpaceParity);
		break;
	case 4:
		port->setParity(QSerialPort::MarkParity);
		break;
	}
}
void MainWindow::setStopBits(int index) {
    switch(index) {
	case 0:
		port->setStopBits(QSerialPort::OneStop);
		break;
	case 1:
		port->setStopBits(QSerialPort::OneAndHalfStop);
		break;
	case 2:
		port->setStopBits(QSerialPort::TwoStop);
		break;
	}
}
