#include "AudioThread.h"
#include <QDebug>
#include <QFile>

extern "C"
{
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}


#ifdef Q_OS_WIN
    // 格式名称
    #define FMT_NAME "dshow"
    #define DEVICE_NAME "audio=麦克风 (2- EDIFIER G4)"
    #define FILENAME "E:/out.pcm"
#else
    #define FMT_NAME "avfoundation"
    #define DEVICE_NAME ":0"
#endif

AudioThread::AudioThread(QObject *parent) : QThread(parent)
{
    connect(this, &AudioThread::finished,
            this, &AudioThread::deleteLater);
}

AudioThread::~AudioThread()
{
    requestInterruption();
    quit();
    wait();
}

void AudioThread::run()
{
    AVInputFormat *fmt = av_find_input_format(FMT_NAME);
    if (!fmt)
    {
        qDebug() << "找不到输入格式" << FMT_NAME;
        return;
    }

    AVFormatContext *ctx = nullptr;
    int ret = avformat_open_input(&ctx, DEVICE_NAME, fmt, nullptr);
    if (ret < 0)
    {
        char errbuf[1024] = {0};
        av_strerror(ret, errbuf, sizeof (errbuf));
        qDebug() << "打开设备失败" << errbuf;
        return;
    }

    QFile file(FILENAME);
    if (!file.open(QFile::WriteOnly))
    {
        qDebug() << "文件打开失败" << FILENAME;
        avformat_close_input(&ctx);
        return;
    }

    AVPacket pkt;

    while (!isInterruptionRequested() && av_read_frame(ctx, &pkt) == 0)
    {
        file.write((const char *)pkt.data, pkt.size);
    }

    file.close();

    avformat_close_input(&ctx);
}
