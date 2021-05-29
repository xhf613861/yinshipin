#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "YuvPlayer.h"

static int yuvIdx = 0;
static Yuv yuvs[] = {
    {
        "E:/demo.yuv",
        620, 252,
        AV_PIX_FMT_YUV420P,
        30
    },
    {
        "E:/yellow1.yuv",
        2560, 1280,
        AV_PIX_FMT_YUV420P,
        30
    },
    {
        "E:/yellow2.yuv",
        1920, 1080,
        AV_PIX_FMT_YUV420P,
        30
    },
//    {
//        "E:/demo.yuv",
//        852, 480,
//        AV_PIX_FMT_YUV420P,
//        30
//    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 创建播放器
    _player = new YuvPlayer(this);

    // 监听播放器
    connect(_player, &YuvPlayer::stateChanged,
            this, &MainWindow::onPlayerStateChanged);

    int w = 500;
    int h = 500;
    int x = (width() - w) >> 1;
    int y = (height() - h) >> 1;
    _player->setGeometry(x, y, w, h);

    // 设置需要播放的文件
    _player->setYuv(yuvs[yuvIdx]);
    _player->play();
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
    }
    else
    {
        _player->play();
    }
}

void MainWindow::on_pushButton_stop_clicked()
{
    _player->stop();
}

void MainWindow::onPlayerStateChanged()
{
    if (_player->getState() == YuvPlayer::Playing)
    {
        ui->pushButton_play->setText("暂停");
    }
    else
    {
        ui->pushButton_play->setText("播放");
    }
}

void MainWindow::on_pushButton_next_clicked()
{
    int yuvCount = sizeof (yuvs) / sizeof (Yuv);
    yuvIdx = ++yuvIdx % yuvCount;

    _player->stop();
    _player->setYuv(yuvs[yuvIdx]);
    _player->play();
}
