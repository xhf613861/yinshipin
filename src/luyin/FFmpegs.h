#ifndef FFMPEGS_H
#define FFMPEGS_H

extern "C"
{
//#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
}

#define AUDIO_FORMAT_PCM 1
#define AUDIO_FORMAT_FLOAT 3

#define ERROR_BUFF(ret) \
    char errbuf[1024]; \
    av_strerror(ret, errbuf, sizeof (errbuf));

typedef struct
{
    uint8_t riffChunkID[4] = {'R', 'I', 'F', 'F'};
    // 整个数据长度 - 8字节
    uint32_t riffChunkSize;
    uint8_t riffFormat[4] = {'W', 'A', 'V', 'E'};

    uint8_t riffSubchunkID[4] = {'f', 'm', 't', ' '};
    // 子数据块长度 16字节
    uint32_t riffSubchunkSize = 16;
    // 固定为1
    uint16_t riffAudioFormat = AUDIO_FORMAT_PCM;
    // 省道数
    uint16_t riffNumChannels;
    // 采样率
    uint32_t riffSampleRate;
    // 字节率 = 采样率 * 位深度 * 声道数 >> 8
    uint32_t riffByteRate;
    // 一个样本字节数 = 位深度 * 声道数 >> 3
    uint16_t riffBlockAlign;
    // 位深度
    uint16_t riffBitsPerSample;

    uint8_t riffSubchunkDataId[4] = {'d', 'a', 't', 'a'};
    uint32_t riffSubchunkDataSize;

} WAVHeader;

typedef struct
{
    const char *filename;
    int sampleRate;
    AVSampleFormat sampleFmt;
    int chLayout;
}AudioEncodeSpec;

typedef struct
{
    const char *filename;
    int width;
    int height;
    AVPixelFormat pixFmt;
    int fps;
}VideoEncodeSpec;


class FFmpegs
{
public:
    FFmpegs();

    static void pcm2wav(WAVHeader &header,
                        const char *pcmFilename,
                        const char *wavFilename);

    static void aacEncode(AudioEncodeSpec &in,
                          const char *outFilename);

    static void aacDecode(const char *inFilename,
                          AudioEncodeSpec &out);

    static void h264Encode(VideoEncodeSpec &in, const char *outFilename);
};



#endif // FFMPEGS_H
