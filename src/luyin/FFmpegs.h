#ifndef FFMPEGS_H
#define FFMPEGS_H

extern "C"
{
//#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
//#include <libavutil/avutil.h>
}

#define AUDIO_FORMAT_PCM 1
#define AUDIO_FORMAT_FLOAT 3

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


class FFmpegs
{
public:
    FFmpegs();

    static void pcm2wav(WAVHeader &header,
                        const char *pcmFilename,
                        const char *wavFilename);
};






//typedef struct
//{
//    uint8_t riffChunkID[4] = {'R', 'I', 'F', 'F'};
//    // 整个数据长度 - 8字节
//    uint32_t riffChunkDataSize;
//    uint8_t format[4] = {'W', 'A', 'V', 'E'};

//    uint8_t fmtChunkId[4] = {'f', 'm', 't', ' '};
//    // 子数据块长度 16字节
//    uint32_t fmtChunkDataSize = 16;
//    // 固定为1
//    uint16_t audioFormat = AUDIO_FORMAT_PCM;
//    // 省道数
//    uint16_t numChannels;
//    // 采样率
//    uint32_t sampleRate;
//    // 字节率 = 采样率 * 位深度 * 声道数 >> 8
//    uint32_t byteRate;
//    // 一个样本字节数 = 位深度 * 声道数 >> 3
//    uint16_t blockAlign;
//    // 位深度
//    uint16_t bitsPerSample;

//    uint8_t dataChunkId[4] = {'d', 'a', 't', 'a'};
//    uint32_t dataChunkDataSize;

//} WAVHeader;


















//// WAV文件头（44字节）
//typedef struct {
//    // RIFF chunk的id
//    uint8_t riffChunkId[4] = {'R', 'I', 'F', 'F'};
//    // RIFF chunk的data大小，即文件总长度减去8字节
//    uint32_t riffChunkDataSize;

//    // "WAVE"
//    uint8_t format[4] = {'W', 'A', 'V', 'E'};

//    /* fmt chunk */
//    // fmt chunk的id
//    uint8_t fmtChunkId[4] = {'f', 'm', 't', ' '};
//    // fmt chunk的data大小：存储PCM数据时，是16
//    uint32_t fmtChunkDataSize = 16;
//    // 音频编码，1表示PCM，3表示Floating Point
//    uint16_t audioFormat = AUDIO_FORMAT_PCM;
//    // 声道数
//    uint16_t numChannels;
//    // 采样率
//    uint32_t sampleRate;
//    // 字节率 = sampleRate * blockAlign
//    uint32_t byteRate;
//    // 一个样本的字节数 = bitsPerSample * numChannels >> 3
//    uint16_t blockAlign;
//    // 位深度
//    uint16_t bitsPerSample;

//    /* data chunk */
//    // data chunk的id
//    uint8_t dataChunkId[4] = {'d', 'a', 't', 'a'};
//    // data chunk的data大小：音频数据的总长度，即文件总长度减去文件头的长度(一般是44)
//    uint32_t dataChunkDataSize;
//} WAVHeader;
#endif // FFMPEGS_H
