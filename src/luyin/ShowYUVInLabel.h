#ifndef SHOWYUVINLABEL_H
#define SHOWYUVINLABEL_H

#include <QWidget>

namespace Ui {
class ShowYUVInLabel;
}

class ShowYUVInLabel : public QWidget
{
    Q_OBJECT

public:
    explicit ShowYUVInLabel(QWidget *parent = nullptr);
    ~ShowYUVInLabel();

    void play();

private:
    Ui::ShowYUVInLabel *ui;

    QWidget *_widget;

};

#endif // SHOWYUVINLABEL_H
