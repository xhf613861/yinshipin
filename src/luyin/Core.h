#ifndef CORE_H
#define CORE_H

#ifdef Q_OS_WIN
    // 格式名称
    #define FMT_NAME "dshow"
    #define DEVICE_NAME "audio=麦克风 (USB Audio Device)"
    #define FILENAME "E:/out.pcm"

    #define DEVICE_VIDEO_NAME "video=PC Camera"
    #define FILENAME_VIDEO "E:/out.yuv"
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
#define BUFFER_SIZE (SAMPLES * BYTES_PER_SAMPLE)

#endif // CORE_H
