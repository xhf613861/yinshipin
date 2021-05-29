#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "AudioThread.h"
#include "PlayThread.h"
#include "FFmpegs.h"
#include "EncodeThread.h"
#include "DecodeThread.h"
#include "ShowBMPThread.h"
#include "CustomDrawThread.h"
#include "VideoThread.h"
#include "ShowYUVThread.h"

#include <ResampleThread.h>

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


void MainWindow::on_pushButton_Audio_clicked()
{
    if (_pAudioThread == nullptr)
    {
        _pAudioThread = new AudioThread(this);

        connect(_pAudioThread, &AudioThread::finished, [this](){
            _pAudioThread = nullptr;
            ui->pushButton_Audio->setText("开始录音");
        });

        connect(_pAudioThread, &AudioThread::audioTime, [this](int audioTime){
            ui->label_AudioTime->setText(QString::number(audioTime));
        });

        _pAudioThread->start();
        ui->pushButton_Audio->setText("停止录音");
    }
    else
    {
        _pAudioThread->requestInterruption();
        _pAudioThread = nullptr;
        ui->pushButton_Audio->setText("开始录音");
    }
}

void MainWindow::on_pushButton_Play_clicked()
{
    if (_pPlayThread == nullptr)
    {
        _pPlayThread = new PlayThread(this);
        connect(_pPlayThread, &PlayThread::finished, [this](){
            _pPlayThread = nullptr;
            ui->pushButton_Play->setText("开始播放");
        });

        connect(_pPlayThread, &PlayThread::notifyAllTime, [this](int allTime){
            ui->label_AllTime->setText(QString("%1毫秒").arg(allTime));
        });

        connect(_pPlayThread, &PlayThread::notifyRemainTime, [this](int remainTime){
            ui->label_hasTime->setText(QString("%1毫秒").arg(remainTime));
        });

        _pPlayThread->start();
        ui->pushButton_Play->setText("停止播放");
    }
    else
    {
        _pPlayThread->requestInterruption();
        _pPlayThread = nullptr;
        ui->pushButton_Play->setText("开始播放");
    }
}

void MainWindow::on_pushButton_Wav_clicked()
{
    // 封装WAV的头部
    WAVHeader header;
    header.riffNumChannels = 2;
    header.riffSampleRate = 44100;
    header.riffBitsPerSample = 16;

//    header.numChannels = 2;
//    header.sampleRate = 44100;
//    header.bitsPerSample = 16;

    // 调用函数
    FFmpegs::pcm2wav(header, "E:/out.pcm", "E:/out.wav");

}

void MainWindow::on_pushButton_Resample_clicked()
{
    ResampleThread *pResampleThread = new ResampleThread;
    pResampleThread->start();
}

void MainWindow::on_pushButton_aacEncode_clicked()
{
    if (_pEncodeThread == nullptr)
    {
        _pEncodeThread = new EncodeThread(this);

        connect(_pEncodeThread, &EncodeThread::finished, [this](){
            _pEncodeThread = nullptr;
            ui->pushButton_aacEncode->setText("aac编码");
        });

        _pEncodeThread->start();
        ui->pushButton_aacEncode->setText("编码中");
    }
}

void MainWindow::on_pushButton_aacDcode_clicked()
{
    if (_pDcodeThread == nullptr)
    {
        _pDcodeThread = new DecodeThread(this);

        connect(_pDcodeThread, &DecodeThread::finished, [this](){
            _pDcodeThread = nullptr;
            ui->pushButton_aacDcode->setText("aac解码");
        });

        _pDcodeThread->start();
        ui->pushButton_aacDcode->setText("解码中");
    }
}

void MainWindow::on_pushButton_showBMP_clicked()
{
    ShowBMPThread *pThread = new ShowBMPThread(this);
    pThread->start();

}

void MainWindow::on_pushButton_customDraw_clicked()
{
    CustomDrawThread *pThread = new CustomDrawThread(this);
    pThread->start();
}

void MainWindow::on_pushButton_video_clicked()
{
    if (_pVideoThread == nullptr)
    {
        _pVideoThread = new VideoThread(this);

        connect(_pVideoThread, &VideoThread::finished, [this](){
            _pVideoThread = nullptr;
            ui->pushButton_video->setText("录制视频");
        });

        _pVideoThread->start();
        ui->pushButton_video->setText("结束录制");
    }
    else
    {
        _pVideoThread->requestInterruption();
        _pVideoThread = nullptr;
        ui->pushButton_video->setText("录制视频");
    }
}

void MainWindow::on_pushButton_showYUV_clicked()
{
   if (_pShowYUVThread == nullptr)
   {
       _pShowYUVThread = new ShowYUVThread(this);

       connect(_pShowYUVThread, &ShowYUVThread::finished, [this](){
           _pShowYUVThread = nullptr;
           ui->pushButton_video->setText("录制视频");
       });

       _pShowYUVThread->start();
   }
}

void MainWindow::on_pushButton_showYUVLabel_clicked()
{

}
