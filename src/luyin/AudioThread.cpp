#include "AudioThread.h"
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
    // 格式名称
    #define FMT_NAME "dshow"
    #define DEVICE_NAME "audio=麦克风 (2- EDIFIER G4)"
    #define FILENAME "E:/out.pcm"
#else
    #define FMT_NAME "avfoundation"
    #define DEVICE_NAME ":0"
#endif

/* 一些宏定义 */
// 采样率
#define SAMPLE_RATE 44100
// 采样格式
#define SAMPLE_FORMAT AUDIO_S16LSB
// 采样大小
#define SAMPLE_SIZE SDL_AUDIO_BITSIZE(SAMPLE_FORMAT)
// 声道数
#define CHANNELS 2
// 音频缓冲区的样本数量
#define SAMPLES 1024

// 每个样本占用多少个字节
#define BYTES_PER_SAMPLE ((SAMPLE_SIZE * CHANNELS) / 8)
// 文件缓冲区的大小
//#define BUFFER_SIZE 4096
#define BUFFER_SIZE (SAMPLES * BYTES_PER_SAMPLE)

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

    qDebug() << "xiancheng xigou";
}

// 用于存储读取的音频数据和长度
typedef struct {
    int len = 0;
    int pullLen = 0;
    Uint8 *data = nullptr;
} AudioBuffer;

int buffLen;
char *buffData;

// userdata：SDL_AudioSpec.userdata
// stream：音频缓冲区（需要将音频数据填充到这个缓冲区）
// len：音频缓冲区的大小（SDL_AudioSpec.samples * 每个样本的大小）
void pull_audio_data(void *userdata, Uint8 *stream, int len) {
    // 清空stream
    SDL_memset(stream, 0, len);

    if (buffLen <= 0)
        return;

    len = (len > buffLen) ? buffLen : len;

    SDL_MixAudio(stream, (Uint8 *)buffData, len, SDL_MIX_MAXVOLUME);

    qDebug() << QThread::currentThreadId();
    buffData += len;
    buffLen -= len;
}

void AudioThread::run()
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



    // 开始播放
    SDL_PauseAudio(0);

    // 存放文件数据
    char data[BUFFER_SIZE];

    while (!isInterruptionRequested()) {
        // 只要从文件中读取的音频数据，还没有填充完毕，就跳过
        buffLen = file.read((char *) data, BUFFER_SIZE);
        if (buffLen <= 0)
            break;

        qDebug() << QThread::currentThreadId();

        buffData = data;

        while (buffLen > 0)
        {
            SDL_Delay(1);
        }
    }

    // 关闭文件
    file.close();
    // 关闭音频设备
    SDL_CloseAudio();
    // 清理所有初始化的子系统
    SDL_Quit();
}
