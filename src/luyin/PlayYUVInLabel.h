#ifndef PLAYYUVINLABEL_H
#define PLAYYUVINLABEL_H

#include <QThread>

class PlayYUVInLabel : public QThread
{
    Q_OBJECT
public:
    PlayYUVInLabel(void *winid, QObject *parent = nullptr);
    virtual ~PlayYUVInLabel();

private:
    void run() override;

private:
    void *_winId;
};

#endif // PLAYYUVINLABEL_H
