#include "ResampleThread.h"

#include <QDebug>
#include <QFile>
#include "FFmpegs.h"

// 导入头文件
extern "C"
{
#include <libswresample/swresample.h>
#include <libavutil/avutil.h>
}



ResampleThread::ResampleThread()
{
    connect(this, &ResampleThread::finished,
            this, &ResampleThread::deleteLater);
}

ResampleThread::~ResampleThread()
{
    requestInterruption();
    quit();
    wait();

    qDebug() << "ResampleThread xiancheng xigou";
}

void ResampleThread::run()
{
    // 创建重采样上下文
//    int64_t out_ch_layout, enum AVSampleFormat out_sample_fmt, int out_sample_rate,
//    int64_t  in_ch_layout, enum AVSampleFormat  in_sample_fmt, int  in_sample_rate,
    int len = 0;

    QString inFileName = "E:/out44100.pcm";
    QString outFileName = "E:/out48000.pcm";
    QFile inFile(inFileName);
    QFile outFile(outFileName);

    // 输出参数
    int64_t outChannel = AV_CH_LAYOUT_MONO;
    AVSampleFormat outSampleFmt = AV_SAMPLE_FMT_FLT;
    int outSampleRate = 48000;

    uint8_t **outAudioData = nullptr;
    int outLinesize;
    int outSamplesCount = 1024;
    int outChns = av_get_channel_layout_nb_channels(outChannel);
    int outBytesPerSample = outChns * av_get_bytes_per_sample(outSampleFmt);


    // 输入参数
    int64_t inChannel = AV_CH_LAYOUT_STEREO;
    AVSampleFormat inSampleFmt = AV_SAMPLE_FMT_S16;
    int inSampleRate = 44100;

    uint8_t **inAudioData = nullptr;
    int inLinesize;
    int inSamplesCount = 1024;
    int inChns = av_get_channel_layout_nb_channels(inChannel);
    int inBytesPerSample = inChns * av_get_bytes_per_sample(inSampleFmt);

    SwrContext *ctx = swr_alloc_set_opts(nullptr,
                                         outChannel, outSampleFmt, outSampleRate,
                                         inChannel, inSampleFmt, inSampleRate,
                                         0, nullptr);
    if (!ctx)
    {
        qDebug() << "swr_alloc_set_opts error";
        return;
    }

    // 初始化重采样上下文
    int ret = swr_init(ctx);
    if (ret < 0)
    {
        ERROR_BUFF(ret);
        qDebug() << "swr_init error" << errbuf;
        goto end;
    }

    // 创建输入缓冲区

    ret = av_samples_alloc_array_and_samples(&inAudioData, &inLinesize, inChns,
                                       inSamplesCount, inSampleFmt, 1);
    if (ret < 0)
    {
        ERROR_BUFF(ret);
        qDebug() << errbuf;
        goto end;
    }

    // 创建输出缓冲区
    ret = av_samples_alloc_array_and_samples(&outAudioData, &outLinesize, outChns,
                                       outSamplesCount, outSampleFmt, 1);

    if (ret < 0)
    {
        ERROR_BUFF(ret);
        qDebug() << errbuf;
        goto end;
    }

    if (!inFile.open(QFile::ReadOnly))
    {
        qDebug() << "inFile Open Failer";
        goto end;
    }

    if (!outFile.open(QFile::WriteOnly))
    {
        qDebug() << "outFile Open Failer";
        goto end;
    }

    //
    while ((len = inFile.read((char *)inAudioData[0], inLinesize)) > 0)
    {
        // 重采样
        inSamplesCount = len / inBytesPerSample;
        // 返回值 转换后的样本数量
        ret = swr_convert(ctx, outAudioData, outSamplesCount,
                    (const uint8_t **)inAudioData, inSamplesCount);

        if (ret < 0)
        {
            ERROR_BUFF(ret);
            qDebug() << "swr_convert error";
            goto end;
        }

        // 将转换后的数据写入到文件
        outFile.write((char*)outAudioData[0], ret * outBytesPerSample);
    }

end:
    // 释放资源
    // 释放重采样上下文

    inFile.close();
    outFile.close();

    if (outAudioData)
        av_freep(&outAudioData[0]);
    av_freep(&outAudioData);

    if (inAudioData)
        av_freep(&inAudioData[0]);
    av_freep(&inAudioData);

    swr_free(&ctx);

    //return ret < 0;
}
