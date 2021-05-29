#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class AudioThread;
class PlayThread;
class EncodeThread;
class DecodeThread;
class VideoThread;
class ShowYUVThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Audio_clicked();

    void on_pushButton_Play_clicked();

    void on_pushButton_Wav_clicked();

    void on_pushButton_Resample_clicked();

    void on_pushButton_aacEncode_clicked();

    void on_pushButton_aacDcode_clicked();

    void on_pushButton_showBMP_clicked();

    void on_pushButton_customDraw_clicked();

    void on_pushButton_video_clicked();

    void on_pushButton_showYUV_clicked();

    void on_pushButton_showYUVLabel_clicked();

private:
    Ui::MainWindow *ui;

    AudioThread *_pAudioThread = nullptr;
    PlayThread *_pPlayThread = nullptr;
    EncodeThread *_pEncodeThread = nullptr;
    DecodeThread *_pDcodeThread = nullptr;
    VideoThread *_pVideoThread = nullptr;

    ShowYUVThread *_pShowYUVThread = nullptr;
};
#endif // MAINWINDOW_H
