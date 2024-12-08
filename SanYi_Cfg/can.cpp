#include "can.h"
#include "ui_can.h"

Can::Can(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Can)
{
    ui->setupUi(this);
    for(int i = 0; i < 10; ++i)
    {
        ui->combo_addr->insertItem(i,QString("can%1").arg(i+1));
    }
    ui->combo_addr->setCurrentIndex(0);
}

Can::~Can()
{
    delete ui;
}

QJsonObject Can::GetCfgJson()
{
    QJsonObject obj;
    obj.insert("can",ui->combo_addr->currentIndex());
    obj.insert("baud",ui->le_baud->text().toInt());
    return obj;
}

void Can::SetCfgJson(const QJsonObject& obj)
{
    ui->combo_addr->setCurrentIndex(obj["can"].toInt());
    ui->le_baud->setText(QString("%1").arg(obj["baud"].toInt()));
}
