#include "VideoEncodeThread.h"

#include <QDebug>
#include "FFmpegs.h"

VideoEncodeThread::VideoEncodeThread(QObject *parent) : QThread(parent)
{
    connect(this, &VideoEncodeThread::finished,
            this, &VideoEncodeThread::deleteLater);
}

VideoEncodeThread::~VideoEncodeThread()
{
    requestInterruption();
    quit();
    wait();

    qDebug() << "VideoEncodeThread xiancheng xigou";
}

void VideoEncodeThread::run()
{
    VideoEncodeSpec in;
    in.filename = "E:/yellow1.yuv";
    in.width = 2560;
    in.height = 1280;
    in.fps = 25;
    in.pixFmt = AV_PIX_FMT_YUV420P;

    FFmpegs::h264Encode(in, "E:/out1.h264");
}
