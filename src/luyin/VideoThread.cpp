#include "VideoThread.h"

#include <QDebug>
#include <QFile>
#include <QTime>
#include "Core.h"
#include "FFmpegs.h"

extern "C"
{
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
//#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
}

VideoThread::VideoThread(QObject *parent) : QThread(parent)
{
    connect(this, &VideoThread::finished,
            this, &VideoThread::deleteLater);
}

VideoThread::~VideoThread()
{
    requestInterruption();
    quit();
    wait();

    qDebug() << "VideoThread xigou";
}

void VideoThread::run()
{
    AVInputFormat *fmt = av_find_input_format(FMT_NAME);
    if (!fmt) {
        // 如果找不到输入格式
        qDebug() << "找不到输入格式" << FMT_NAME;
        return;
    }

    // 格式上下文（后面通过格式上下文操作设备）
    AVFormatContext *ctx = nullptr;

    // 传递给输入设备的参数
    AVDictionary *options = nullptr;
    av_dict_set(&options, "videos_size", "640x480", 0);
    av_dict_set(&options, "pixel_format", "yuyv422", 0);
    av_dict_set(&options, "framerate", "25", 0);

    // 打开设备
    int ret = avformat_open_input(&ctx, DEVICE_VIDEO_NAME, fmt, &options);
    // 如果打开设备失败
    if (ret < 0) {
        char errbuf[1024] = {0};
        // 根据函数返回的错误码获取错误信息
        av_strerror(ret, errbuf, sizeof (errbuf));
        qDebug() << "打开设备失败" << errbuf;
        return;
    }

    // 文件
    QFile file(FILENAME_VIDEO);

    // WriteOnly：只写模式。如果文件不存在，就创建文件；如果文件存在，就删除文件内容
    if (!file.open(QFile::WriteOnly)) {
        qDebug() << "文件打开失败" << FILENAME_VIDEO;
        // 关闭设备
        avformat_close_input(&ctx);
        return;
    }


    // 计算每一帧的大小
    AVCodecParameters *params = ctx->streams[0]->codecpar;
    int imageSize = av_image_get_buffer_size(
                (AVPixelFormat) params->format,
                params->width, params->height,1);

    // 数据包
    AVPacket *pkt = av_packet_alloc();
    // 从设备中采集数据
    // av_read_frame返回值为0，代表采集数据成功
    while (!isInterruptionRequested()) {

        // 不断采集数据
        ret = av_read_frame(ctx, pkt);
        if (ret == 0)
        {
            file.write((const char *)pkt->data, imageSize);

            /**
              这里要使用imageSize,而不是pkt->size
              pkt->size有可能比imageSize大（比如在mac平台)
              使用pkt->size会导致谢图一些多余数据到YUV文件中
              进而导致YUV内容无法正常播放
              */

            // 释放资源
            av_packet_unref(pkt);
        }
        else if (ret == AVERROR(EAGAIN))
        {
            // 临时资源不可用
            continue;
        }
        else
        {
            ERROR_BUFF(ret);
            qDebug() << "av_read_frame error" << errbuf;
            return ;
        }
    }

    // 释放资源
    av_packet_free(&pkt);

    // 关闭文件
    file.close();

    // 关闭设备
    avformat_close_input(&ctx);

}
