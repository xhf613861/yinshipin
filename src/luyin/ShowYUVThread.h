#ifndef SHOWYUVTHREAD_H
#define SHOWYUVTHREAD_H

#include <QThread>

class ShowYUVThread : public QThread
{
public:
    ShowYUVThread(QObject *parent = nullptr);
    virtual ~ShowYUVThread();

private:
    void run() override;
};

#endif // SHOWYUVTHREAD_H
