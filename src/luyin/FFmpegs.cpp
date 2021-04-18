#include "FFmpegs.h"
#include <QFile>
#include <QDebug>

//extern "C"
//{
////#include <libavdevice/avdevice.h>
//#include <libavformat/avformat.h>
////#include <libavutil/avutil.h>
//}

FFmpegs::FFmpegs()
{

}

void FFmpegs::pcm2wav(WAVHeader &header, const char *pcmFilename, const char *wavFilename)
{
    QFile pcmFile(pcmFilename);
    if (!pcmFile.open(QFile::ReadOnly))
    {
        qDebug() << "PCM文件打开失败。";
        return;
    }

    QFile wavFile(wavFilename);
    if (!wavFile.open(QFile::WriteOnly))
    {
        qDebug() << "WAC文件打开失败。";
        pcmFile.close();
        return;
    }

    header.riffChunkSize = pcmFile.size() + 44 - 8;
    header.riffBlockAlign = header.riffBitsPerSample * header.riffNumChannels >> 3;
    header.riffByteRate = header.riffSampleRate * header.riffBlockAlign;
    header.riffSubchunkDataSize = pcmFile.size();

    int tmp = sizeof (WAVHeader);
//    header.riffSubchunkDataId[0] = 'd';
//    header.riffSubchunkDataId[1] = 'd';
//    header.riffSubchunkDataId[2] = 'd';
//    header.riffSubchunkDataId[3] = 'd';
    wavFile.write((const char *) &header, sizeof (WAVHeader));

    char buff[1024];
    int nLength = 0;
    while ((nLength = pcmFile.read(buff, 1024)) > 0)
    {
        wavFile.write(buff, nLength);
    }

    wavFile.close();
    pcmFile.close();
}









































//void FFmpegs::pcm2wav(WAVHeader &header, const char *pcmFilename, const char *wavFilename)
//{
//    header.blockAlign = header.bitsPerSample * header.numChannels >> 3;
//    header.byteRate = header.sampleRate * header.blockAlign;

//    // 打开pcm文件
//    QFile pcmFile(pcmFilename);
//    if (!pcmFile.open(QFile::ReadOnly)) {
//        qDebug() << "文件打开失败" << pcmFilename;
//        return;
//    }
//    header.dataChunkDataSize = pcmFile.size();
//    header.riffChunkDataSize = header.dataChunkDataSize
//                               + sizeof (WAVHeader) - 8;

//    // 打开wav文件
//    QFile wavFile(wavFilename);
//    if (!wavFile.open(QFile::WriteOnly)) {
//        qDebug() << "文件打开失败" << wavFilename;

//        pcmFile.close();
//        return;
//    }

//    // 写入头部
//    wavFile.write((const char *) &header, sizeof (WAVHeader));

//    // 写入pcm数据
//    char buf[1024];
//    int size;
//    while ((size = pcmFile.read(buf, sizeof (buf))) > 0) {
//        wavFile.write(buf, size);
//    }

//    // 关闭文件
//    pcmFile.close();
//    wavFile.close();
//}
