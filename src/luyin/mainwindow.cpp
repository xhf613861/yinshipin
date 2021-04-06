#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "AudioThread.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if (_pAudioThread == nullptr)
    {
        _pAudioThread = new AudioThread(this);
//        connect(_pAudioThread, &AudioThread::finished, [this](){
//            _pAudioThread = nullptr;
//            ui->pushButton->setText("开始录音");
//        });
        _pAudioThread->start();
        ui->pushButton->setText("结束录音");
    }
    else
    {
        _pAudioThread->requestInterruption();
    }
}
