#ifndef DECODETHREAD_H
#define DECODETHREAD_H

#include <QThread>

class DecodeThread : public QThread
{
public:
    DecodeThread(QObject *parent);

private:
    void run() override;
};

#endif // DECODETHREAD_H
