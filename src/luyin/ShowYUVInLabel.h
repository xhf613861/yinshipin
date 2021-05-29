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

private:
    Ui::ShowYUVInLabel *ui;
};

#endif // SHOWYUVINLABEL_H
