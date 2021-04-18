#include "PlayThread.h"
#include "Core.h"

#include <QDebug>
#include <QFile>

extern "C"
{
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <SDL2/SDL.h>
}

#ifdef Q_OS_WIN
    #define FILENAME "E:/out.pcm"
#else
    #define FMT_NAME "avfoundation"
#endif



PlayThread::PlayThread(QObject *parent) : QThread(parent)
{
    connect(this, &PlayThread::finished,
            this, &PlayThread::deleteLater);
}

PlayThread::~PlayThread()
{
    requestInterruption();
    quit();
    wait();

    qDebug() << "xiancheng xigou";
}

// 用于存储读取的音频数据和长度
typedef struct {
    int len = 0;
    int pullLen = 0;
    Uint8 *data = nullptr;
} AudioBuffer;

// userdata：SDL_AudioSpec.userdata
// stream：音频缓冲区（需要将音频数据填充到这个缓冲区）
// len：音频缓冲区的大小（SDL_AudioSpec.samples * 每个样本的大小）
void pull_audio_data(void *userdata, Uint8 *stream, int len) {
    // 清空stream
    SDL_memset(stream, 0, len);

    AudioBuffer *bufferData = (AudioBuffer *)userdata;
    if (bufferData->len <= 0)
        return;

    len = (len > bufferData->len) ? bufferData->len : len;

    SDL_MixAudio(stream, bufferData->data, len, SDL_MIX_MAXVOLUME);

    qDebug() << QThread::currentThreadId();
    bufferData->data += len;
    bufferData->len -= len;
    bufferData->pullLen = len;
}


void PlayThread::run()
{
    // 初始化Audio子系统
    if (SDL_Init(SDL_INIT_AUDIO)) {
        // 返回值不是0，就代表失败
        qDebug() << "SDL_Init Error" << SDL_GetError();
        return;
    }

    // 音频参数
    SDL_AudioSpec spec;
    // 采样率
    spec.freq = SAMPLE_RATE;
    // 采样格式（s16le）
    spec.format = SAMPLE_FORMAT;
    // 声道数
    spec.channels = CHANNELS;
    // 音频缓冲区的样本数量（这个值必须是2的幂）
    spec.samples = SAMPLES;
    // 回调
    spec.callback = pull_audio_data;
    // 传递给回调的参数
    AudioBuffer buffer;
    spec.userdata = &buffer;

    // 打开音频设备
    if (SDL_OpenAudio(&spec, nullptr)) {
        qDebug() << "SDL_OpenAudio Error" << SDL_GetError();
        // 清除所有初始化的子系统
        SDL_Quit();
        return;
    }

    // 打开文件
    QFile file(FILENAME);
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "文件打开失败" << FILENAME;
        // 关闭音频设备
        SDL_CloseAudio();
        // 清除所有初始化的子系统
        SDL_Quit();
        return;
    }

    int nRemainFileSize = file.size();
    emit notifyAllTime(getTimeByDataLength(nRemainFileSize));

    // 开始播放
    SDL_PauseAudio(0);

    // 存放文件数据
    Uint8 data[BUFFER_SIZE];

    while (!isInterruptionRequested())
    {
        // 只要从文件中读取的音频数据，还没有填充完毕，就跳过
        if (buffer.len > 0)
        {
            continue;
        }

        int nLen = file.read((char *)data, BUFFER_SIZE);
        nRemainFileSize -= nLen;
        emit notifyRemainTime(getTimeByDataLength(nRemainFileSize));
        if (nLen <= 0)
        {
            SDL_Delay(getTimeByDataLength(buffer.pullLen));
            break;
        }

        buffer.data = data;
        buffer.len = nLen;
    }

    // 关闭文件
    file.close();
    // 关闭音频设备
    SDL_CloseAudio();
    // 清理所有初始化的子系统
    SDL_Quit();
}

int PlayThread::getTimeByDataLength(int dataLength)
{
    int sampleCount = dataLength / BYTES_PER_SAMPLE;
    return sampleCount / SAMPLE_RATE;
}
