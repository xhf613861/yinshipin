#ifndef PLAYTHREAD_H
#define PLAYTHREAD_H

#include <QThread>

class PlayThread : public QThread
{
    Q_OBJECT

public:
    PlayThread(QObject *parent);
    virtual ~PlayThread();

signals:
    void notifyAllTime(int allTime);
    void notifyRemainTime(int remainTime);

private:
    void run() override;

    int getTimeByDataLength(int dataLength);
};

#endif // PLAYTHREAD_H
