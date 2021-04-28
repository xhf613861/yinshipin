#ifndef RESAMPLETHREAD_H
#define RESAMPLETHREAD_H
#include <QThread>

class ResampleThread : public QThread
{
public:
    ResampleThread();
    virtual ~ResampleThread();

private:
    void run() override;
};

#endif // RESAMPLETHREAD_H
