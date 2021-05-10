#include "EncodeThread.h"

#include <QDebug>
#include "FFmpegs.h"

#include <QFile>

extern "C"
{
//#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
}

EncodeThread::EncodeThread(QObject *parent) : QThread(parent)
{
    connect(this, &EncodeThread::finished,
            this, &EncodeThread::deleteLater);
}

EncodeThread::~EncodeThread()
{
    requestInterruption();
    quit();
    wait();

    qDebug() << "ResampleThread xiancheng xigou";
}

void EncodeThread::run()
{
    AudioEncodeSpec in;
    in.filename = "E:/out.pcm";
    in.sampleRate = 44100;
    in.sampleFmt = AV_SAMPLE_FMT_S16;
    in.chLayout = AV_CH_LAYOUT_STEREO;

    FFmpegs::aacEncode(in, "E:/out.aac");

}
