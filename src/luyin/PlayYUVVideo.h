#ifndef PLAYYUVVIDEO_H
#define PLAYYUVVIDEO_H

#include <QWidget>
#include <SDL2/SDL.h>
#include <QFile>

namespace Ui {
class PlayYUVVideo;
}

class PlayYUVVideo : public QWidget
{
    Q_OBJECT

public:
    explicit PlayYUVVideo(QWidget *parent = nullptr);
    ~PlayYUVVideo();

private slots:
    void on_pushButton_clicked();

private:
    Ui::PlayYUVVideo *ui;

    QWidget *_widget;
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    SDL_Texture *_texture;
    QFile _file;
    int _timerId;

    void timerEvent(QTimerEvent *event) override;
};

#endif // PLAYYUVVIDEO_H
