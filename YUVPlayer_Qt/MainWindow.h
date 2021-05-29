#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class YuvPlayer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_play_clicked();

    void on_pushButton_stop_clicked();

    void onPlayerStateChanged();

    void on_pushButton_next_clicked();

private:
    Ui::MainWindow *ui;

    YuvPlayer *_player = nullptr;
};
#endif // MAINWINDOW_H
