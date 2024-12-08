#include "di.h"
#include "ui_di.h"

Di::Di(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Di)
{
    ui->setupUi(this);
    for(int i = 0; i < 20; ++i)
    {
        ui->combo_di->insertItem(i,QString("DI%1").arg(i+1));
    }
    ui->combo_di->setCurrentIndex(0);
}

Di::~Di()
{
    delete ui;
}

QJsonObject Di::GetCfgJson()
{
    QJsonObject obj;
    obj.insert("pin",ui->combo_di->currentIndex());
    return obj;
}

void Di::SetCfgJson(const QJsonObject& obj)
{
    ui->combo_di->setCurrentIndex(obj["pin"].toInt());
}
