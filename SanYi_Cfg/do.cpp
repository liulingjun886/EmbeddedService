#include "do.h"
#include "ui_do.h"

Do::Do(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Do)
{
    ui->setupUi(this);
    for(int i = 0; i < 20; ++i)
    {
        ui->combo_do->insertItem(i,QString("DO%1").arg(i+1));
    }
    ui->combo_do->setCurrentIndex(0);
}

Do::~Do()
{
    delete ui;
}

QJsonObject Do::GetCfgJson()
{
    QJsonObject obj;
    obj.insert("pin",ui->combo_do->currentIndex());
    return obj;
}

void Do::SetCfgJson(const QJsonObject& obj)
{
    ui->combo_do->setCurrentIndex(obj["pin"].toInt());
}
