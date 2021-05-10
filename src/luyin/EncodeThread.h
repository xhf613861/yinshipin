#ifndef ENCODETHREAD_H
#define ENCODETHREAD_H

#include <QThread>

class EncodeThread : public QThread
{
public:
    EncodeThread(QObject *parent);
    virtual ~EncodeThread();

private:
    void run() override;
};

#endif // ENCODETHREAD_H
