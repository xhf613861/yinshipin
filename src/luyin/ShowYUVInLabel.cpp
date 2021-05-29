#include "ShowYUVInLabel.h"
#include "ui_ShowYUVInLabel.h"
#include "PlayYUVInLabel.h"

ShowYUVInLabel::ShowYUVInLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowYUVInLabel)
{
    ui->setupUi(this);

    _widget = new QWidget(this);
    _widget->setGeometry(200, 50, 512, 512);
}

ShowYUVInLabel::~ShowYUVInLabel()
{
    delete ui;
}

void ShowYUVInLabel::play()
{
//    PlayYUVInLabel *thread = new PlayYUVInLabel((void *) ui->label->winId(), this);
//    thread->start();

    PlayYUVInLabel *thread = new PlayYUVInLabel((void *) _widget->winId(), this);
    thread->start();
}
