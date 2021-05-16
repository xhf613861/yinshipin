#ifndef CUSTOMDRAWTHREAD_H
#define CUSTOMDRAWTHREAD_H

#include <QThread>

class SDL_Texture;
class SDL_Renderer;

class CustomDrawThread : public QThread
{
public:
    CustomDrawThread(QObject *parent = nullptr);
    virtual ~CustomDrawThread();

private:
    void run() override;
    SDL_Texture *createTexture(SDL_Renderer *rendere);
};

#endif // CUSTOMDRAWTHREAD_H
