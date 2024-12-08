#include "serialport.h"
#include "ui_serialport.h"

SerialPort::SerialPort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPort)
{
    ui->setupUi(this);
    for(int i = 0; i < 20; ++i)
    {
        ui->addr->insertItem(i,QString("RS485-%0").arg(i+1));
    }

    ui->band->insertItem(0,"300");
    ui->band->insertItem(1,"600");
    ui->band->insertItem(2,"1200");
    ui->band->insertItem(3,"2400");
    ui->band->insertItem(4,"4800");
    ui->band->insertItem(5,"9600");
    ui->band->insertItem(6,"19200");
    ui->band->insertItem(7,"38400");
    ui->band->insertItem(8,"57600");
    ui->band->insertItem(9,"115200");
    ui->band->setCurrentIndex(5);

    ui->databit->insertItem(0,"7");
    ui->databit->insertItem(1,"8");
    ui->databit->insertItem(2,"9");
    ui->databit->setCurrentIndex(1);

    ui->partybit->insertItem(0,"无校验");
    ui->partybit->insertItem(1,"奇校验");
    ui->partybit->insertItem(2,"偶校验");
    ui->partybit->setCurrentIndex(0);

    ui->stopbit->insertItem(0,"1");
    ui->stopbit->insertItem(1,"2");
    ui->stopbit->setCurrentIndex(0);
}

SerialPort::~SerialPort()
{
    delete ui;
}

QJsonObject SerialPort::GetCfgJson()
{
    QJsonObject obj;

    obj["addr"] = ui->addr->currentIndex();
    obj["band"] = ui->band->currentText().toInt();
    obj["databit"] = ui->databit->currentText().toInt();
    obj["party"] = ui->partybit->currentIndex();
    obj["stopbit"] = ui->stopbit->currentText().toInt();
    obj["slaveid"] = ui->devaddr->text().toInt();
    return obj;
}

void SerialPort::SetCfgJson(const QJsonObject& obj)
{
    ui->addr->setCurrentIndex(obj["addr"].toInt());
    ui->band->setCurrentText(QString("%1").arg(obj["band"].toInt()));
    ui->databit->setCurrentText(QString("%1").arg(obj["databit"].toInt()));
    ui->partybit->setCurrentIndex(obj["party"].toInt());
    ui->stopbit->setCurrentText(QString("%1").arg(obj["stopbit"].toInt()));
    ui->devaddr->setText(QString("%1").arg(obj["slaveid"].toInt()));
    return;
}
