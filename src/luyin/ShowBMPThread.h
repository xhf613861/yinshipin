#ifndef SHOWBMPTHREAD_H
#define SHOWBMPTHREAD_H

#include <QThread>

class ShowBMPThread : public QThread
{
public:
    ShowBMPThread(QObject *parent = nullptr);
    ~ShowBMPThread();

private:
    void run() override;
};

#endif // SHOWBMPTHREAD_H
