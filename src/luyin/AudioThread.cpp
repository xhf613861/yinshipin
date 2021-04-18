#include "AudioThread.h"
#include <QDebug>
#include <QFile>
#include <QTime>
#include "Core.h"

extern "C"
{
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <SDL2/SDL.h>
}





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

    qDebug() << "AudioThread xigou";
}


void AudioThread::run()
{
    AVInputFormat *fmt = av_find_input_format(FMT_NAME);
    if (!fmt) {
        // 如果找不到输入格式
        qDebug() << "找不到输入格式" << FMT_NAME;
        return;
    }

    // 格式上下文（后面通过格式上下文操作设备）
    AVFormatContext *ctx = nullptr;
    // 打开设备
    int ret = avformat_open_input(&ctx, DEVICE_NAME, fmt, nullptr);
    // 如果打开设备失败
    if (ret < 0) {
        char errbuf[1024] = {0};
        // 根据函数返回的错误码获取错误信息
        av_strerror(ret, errbuf, sizeof (errbuf));
        qDebug() << "打开设备失败" << errbuf;
        return;
    }

    // 文件
    QFile file(FILENAME);

    // WriteOnly：只写模式。如果文件不存在，就创建文件；如果文件存在，就删除文件内容
    if (!file.open(QFile::WriteOnly)) {
        qDebug() << "文件打开失败" << FILENAME;
        // 关闭设备
        avformat_close_input(&ctx);
        return;
    }

    QTime time = QTime::currentTime();

    // 数据包
    AVPacket pkt;
    int nTotalSize = 0;
    // 从设备中采集数据
    // av_read_frame返回值为0，代表采集数据成功
    while (!isInterruptionRequested() && av_read_frame(ctx, &pkt) == 0) {
        // 写入数据
        file.write((const char *) pkt.data, pkt.size);

        nTotalSize += pkt.size;
        int sampleCount = nTotalSize / BYTES_PER_SAMPLE;
        emit audioTime(sampleCount / SAMPLE_RATE);
        //emit audioTime(time.msecsTo(QTime::currentTime()));
    }

    // 关闭文件
    file.close();

    // 关闭设备
    avformat_close_input(&ctx);

}
