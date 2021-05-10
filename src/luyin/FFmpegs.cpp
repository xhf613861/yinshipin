#include "FFmpegs.h"
#include <QFile>
#include <QDebug>

//extern "C"
//{
//#include <libavdevice/avdevice.h>
//#include <libavformat/avformat.h>
//#include <libavutil/avutil.h>
//#include <libavcodec/avcodec.h>
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

static int check_sample_fmt(const AVCodec *codec,
                            enum AVSampleFormat sample_fmt)
{
    const enum AVSampleFormat *p = codec->sample_fmts;
    while (*p != AV_SAMPLE_FMT_NONE)
    {
        if (*p == sample_fmt)
            return 1;

        p++;
    }

    return 0;
}

// 音频编码
// 返回负数：中途出现了错误
// 返回0：编码操作正常完成
static int encode(AVCodecContext *ctx,
                  AVFrame *frame,
                  AVPacket *pkt,
                  QFile &outFile)
{
    // 发送数据到编码器
    int ret = avcodec_send_frame(ctx, frame);
    if (ret < 0)
    {
        ERROR_BUFF(ret);
        qDebug() << "avcode_send_frame error" << errbuf;
        return ret;
    }

    while (true)
    {
        // 从编码器中获取编码后的数据
        ret = avcodec_receive_packet(ctx, pkt);
        // packet 中已经没有数据，需要重新发送数据到编码器（send frame）
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            return 0;
        }
        else if (ret < 0)
        {
            ERROR_BUFF(ret);
            qDebug() << "avcode_receive_packet error" << errbuf;
            return ret;
        }

        // 将编码后的数据写入文件
        outFile.write((char *)pkt->data, pkt->size);

        // 释放资源
        av_packet_unref(pkt);
    }

    return 0;
}

void FFmpegs::aacEncode(AudioEncodeSpec &in, const char *outFileName)
{
    // 编码器
    AVCodec *codec = nullptr;
    // 上下文
    AVCodecContext *ctx = nullptr;

    // 用来存放编码前的数据
    AVFrame *frame = nullptr;
    // 用来存放编码后的数据
    AVPacket *pkt = nullptr;

    int ret = 0;

    QFile inFile(in.filename);
    QFile outFile(outFileName);


    codec = avcodec_find_encoder_by_name("libfdk_aac");
    if (!codec)
    {
        qDebug() << "encoder libfdk_aac not found";
        return;
    }

    // 检查采样格式
    if (!check_sample_fmt(codec, in.sampleFmt))
    {
        qDebug() << "Encoder does not support sample format"
                 << av_get_sample_fmt_name(in.sampleFmt);
        return;
    }

    // 创建上下文
    ctx = avcodec_alloc_context3(codec);
    if (!ctx)
    {
        qDebug() << "avcodec_alloc_context3 error";
        return;
    }

    ctx->sample_fmt = in.sampleFmt;
    ctx->sample_rate = in.sampleRate;
    ctx->channel_layout = in.chLayout;
    // 比特率
    ctx->bit_rate = 32000;
    // 规格
    ctx->profile = FF_PROFILE_AAC_HE_V2;

    // 打开编码器
    ret = avcodec_open2(ctx, codec, nullptr);
    if (ret < 0)
    {
        ERROR_BUFF(ret);
        qDebug() << "avcodec_open2 error" << errbuf;
        goto end;
    }

    // 设置libfdk_aac特有的参数 比如vbr 等命令行参数
//    AVDictionary *options = nullptr;
//    av_dict_set(&options, "vbr", "1", 0);
//    ret = avcodec_open2(ctx, codec, &options);

    frame = av_frame_alloc();
    if (!frame)
    {
        qDebug() << "av_frame_alloc error";
        goto end;
    }

    // 样本帧数量（由frame_size决定，系统决定）
    frame->nb_samples = ctx->frame_size;
    // 采样格式
    frame->format = ctx->sample_fmt;
    // 声道布局
    frame->channel_layout = ctx->channel_layout;
    //frame->sample_rate =
    // 创建AVFrame内部的缓冲区
    ret = av_frame_get_buffer(frame, 0);
    if (ret < 0)
    {
        ERROR_BUFF(ret);
        qDebug() << "av_frame_get_buffer error" << errbuf;
        goto end;
    }

    // 创建ACVPacket
    pkt = av_packet_alloc();
    if (!pkt)
    {
        qDebug() << "av_packet_alloc error";
        goto end;
    }

    if (!inFile.open(QFile::ReadOnly))
    {
        qDebug() << "file open error" << in.filename;
        goto end;
    }

    if (!outFile.open(QFile::WriteOnly))
    {
        qDebug() << "file open error" << outFileName;
        goto end;
    }

    while ((ret = inFile.read((char *)frame->data[0],
                              frame->linesize[0])) > 0)
    {
        if (ret < frame->linesize[0])
        {
            // 声道数
            int chs = av_get_channel_layout_nb_channels(frame->channel_layout);
            // 每个样本的大小
            int bytes = av_get_bytes_per_sample((AVSampleFormat)frame->format);
            // 改为真正有效的样本帧数量
            frame->nb_samples = ret / (chs * bytes);
        }


        // 编码
        if (encode(ctx, frame, pkt, outFile) < 0)
            goto end;
    }

    // flush编码器
    encode(ctx, nullptr, pkt, outFile);

end:
    inFile.close();
    outFile.close();

    // 释放资源
    av_frame_free(&frame);
    av_packet_free(&pkt);
    avcodec_free_context(&ctx);
}

// 输入缓冲区的大小
#define IN_DATA_SIZE 20480

// 需要再次读取输入文件数据的阈值
#define REFILL_THRESH 4096

static int decode(AVCodecContext *ctx,
                  AVPacket *pkt,
                  AVFrame *frame,
                  QFile &outFile)
{
    // 发送压缩数据到解码器
    int ret = avcodec_send_packet(ctx, pkt);
    if (ret < 0)
    {
        ERROR_BUFF(ret);
        qDebug() << "avcodec_send_packet error" << errbuf;
        return ret;
    }

    while (true)
    {
        // 获取解码后的数据
        ret = avcodec_receive_frame(ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            return 0;
        }
        else if (ret < 0)
        {
            ERROR_BUFF(ret);
            qDebug() << "avcodec_receive_frame error" << errbuf;
            return ret;
        }

        outFile.write((char *) frame->data[0], frame->linesize[0]);
    }
}

void FFmpegs::aacDecode(const char *inFilename, AudioEncodeSpec &out)
{
    int ret = 0;

    // 每次输入文件中读取的长度
    int inLen = 0;

    // 是否已经读取到了输入文件的尾部
    bool inEnd = false;

    // 用来存放读取的文件数据
    // 加上AV_INPUT_BUFFER_PADDING_SIZE是为了防止某些优化过的reader一次性读取过多导致越界
    char inDataArray[IN_DATA_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    char *inData = inDataArray;

    QFile inFile(inFilename);
    QFile outFile(out.filename);

    // 编码器
    AVCodec *codec = nullptr;
    // 上下文
    AVCodecContext *ctx = nullptr;
    // 解析器上下文
    AVCodecParserContext *parserCtx = nullptr;

    // 存放解码前的数据
    AVPacket *pkt = nullptr;
    // 存放解码后的数据
    AVFrame *frame = nullptr;

    codec = avcodec_find_decoder_by_name("libfdk_aac");
    if (!codec)
    {
        qDebug() << "decoder libfdk_aac not found";
        return;
    }

    // 初始化解析器上下文
    parserCtx = av_parser_init(codec->id);
    if (!parserCtx)
    {
        qDebug() << "av_parser_init error";
        return;
    }

    // 创建上下文
    ctx = avcodec_alloc_context3(codec);
    if (!ctx)
    {
        qDebug() << "avcodec_alloc_context3 error";
        goto end;
    }

    // 创建AVPacket
    pkt = av_packet_alloc();
    if (!pkt)
    {
        qDebug() << "av_packet_alloc error";
        goto end;
    }

    // 创建AVFrame
    frame = av_frame_alloc();
    if (!frame)
    {
        qDebug() << "av_frame_alloc error";
        goto end;
    }

    // 打开解码器
    ret = avcodec_open2(ctx, codec, nullptr);
    if (ret < 0)
    {
        ERROR_BUFF(ret);
        qDebug() << "avcodec_open2 error" <<errbuf;
        goto end;
    }

    if (!inFile.open(QFile::ReadOnly))
    {
        qDebug() << "file open error:" << inFilename;
        goto end;
    }

    if (!outFile.open(QFile::WriteOnly))
    {
        qDebug() << "file open error:" << out.filename;
        goto end;
    }

    // 读取数据
    inLen = inFile.read(inData, IN_DATA_SIZE);
    while (inLen > 0)
    {
        // 经过解析器上下文处理
        ret = av_parser_parse2(parserCtx, ctx,
                               &pkt->data, &pkt->size,
                               (uint8_t *)inData, inLen,
                               AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
        if (ret < 0)
        {
            ERROR_BUFF(ret);
            qDebug() << "av_parser_parse2 error" << errbuf;
            goto end;
        }

        // 跳过已经解析过的数据
        inData += ret;
        // 减去已经解析过的数据大小
        inLen -= ret;

        // 解码
        if (pkt->size > 0 && decode(ctx, pkt, frame, outFile) < 0)
        {
            goto end;
        }

        // 如果数据不够了，再次读取文件
        if (inLen < REFILL_THRESH && !inEnd)
        {
            // 剩余数据移动到缓冲区前
            memmove(inDataArray, inData, inLen);
            inData = inDataArray;

            // 跨过已有数据，读取文件数据
            int len = inFile.read(inData + inLen, IN_DATA_SIZE - inLen);
            if (len > 0)
            {
                inLen += len;
            }
            else
            {
                inEnd = true;
            }
        }
    }

    // flush 解码器
    // pkt->data = NULL;
    // pkt->size = 0;
    decode(ctx, nullptr, frame, outFile);

    // 设置输出参数
    out.sampleRate = ctx->sample_rate;
    out.sampleFmt = ctx->sample_fmt;
    out.chLayout = ctx->channel_layout;

end:
    inFile.close();
    outFile.close();
    av_parser_close(parserCtx);
    avcodec_free_context(&ctx);
    av_packet_free(&pkt);

    av_frame_free(&frame);
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
