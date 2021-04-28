#ifndef FFMPEGCMD_H
#define FFMPEGCMD_H

/*
 * 查看设备命令
 * ffmpeg -f dshow -list_devices true -i dummy
 * 录音命令
 * ffmpeg -f dshow -i audio="麦克风 (2- EDIFIER G4)" out222.wav
 * 播放命令
 * ffplay out.wav
 * ffplay -ar 44100 -ac 2 -f s16le out.pcm
 * pcm转wav
 * ffmpeg -ar 44100 -ac 2 -f s16le -i out.pcm out.wav
 * ffmpeg -ar 44100 -ac 2 -f s16le -i out.pcm -bitexact out2.wav // 去掉LIST Chunk
 * 音频重采样
 * ffmpeg -ar 44100 -ac 2 -f s16le -i out.pcm -ar 48000 -ac 1 -f f32le 48000_f32le_1.pcm
 * */

#endif // FFMPEGCMD_H
