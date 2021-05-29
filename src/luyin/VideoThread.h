#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>

class VideoThread : public QThread
{
public:
    VideoThread(QObject *parent = nullptr);
    ~VideoThread();

private:
    void run() override;
};

#endif // VIDEOTHREAD_H
