#ifndef VIDEOENCODETHREAD_H
#define VIDEOENCODETHREAD_H

#include <QThread>

class VideoEncodeThread : public QThread
{
public:
    VideoEncodeThread(QObject *parent = nullptr);
    virtual ~VideoEncodeThread();

private:
    void run() override;
};

#endif // VIDEOENCODETHREAD_H
