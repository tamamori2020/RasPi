#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPixmap>
#include <QString>
#include <wiringPi.h>

#define LED0 23

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    onoff = false;
    wiringPiSetupGpio();
    pinMode(LED0, OUTPUT);
    digitalWrite(LED0,false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PushButton_clicked()
{
//    QMessageBox msgBox(this);
//        QString strMsg = "test 1";
//        msgBox.setText(strMsg);
//        msgBox.exec();

    if(!onoff){
        digitalWrite(LED0,true);
        onoff = true;
     }else{
        digitalWrite(LED0,false);
        onoff = false;
    }


}
