#include "ShowYUVInLabel.h"
#include "ui_ShowYUVInLabel.h"

ShowYUVInLabel::ShowYUVInLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowYUVInLabel)
{
    ui->setupUi(this);
}

ShowYUVInLabel::~ShowYUVInLabel()
{
    delete ui;
}
