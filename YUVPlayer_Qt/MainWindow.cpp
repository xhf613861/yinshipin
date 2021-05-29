#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "YuvPlayer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 创建播放器
    _player = new YuvPlayer(this);
    int w = 620;
    int h = 400;
    int x = (width() - w) >> 1;
    int y = (height() - h) >> 1;
    _player->setGeometry(x, y, w, h);

    // 设置需要播放的文件
    Yuv yuv = {
        "E:/demo.yuv",
        620, 252,
        AV_PIX_FMT_YUV420P,
        30
    };

    _player->setYuv(yuv);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_play_clicked()
{
    if (_player->isPlaying())
    {
        _player->pause();
        ui->pushButton_play->setText("播放");
    }
    else
    {
        _player->play();
        ui->pushButton_play->setText("暂停");
    }
}

void MainWindow::on_pushButton_stop_clicked()
{
    _player->stop();
}
