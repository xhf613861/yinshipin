#include "DecodeThread.h"
#include "FFmpegs.h"
#include <QDebug>

DecodeThread::DecodeThread(QObject *parent)
{

}

void DecodeThread::run()
{
    AudioEncodeSpec out;
    out.filename = "E:/out.pcm";
    FFmpegs::aacDecode("E:/in.aac", out);
    // 44100
    qDebug() << out.sampleRate;
    // s16
    qDebug() << av_get_sample_fmt_name(out.sampleFmt);
    // 2
    qDebug() << av_get_channel_layout_nb_channels(out.chLayout);

}
