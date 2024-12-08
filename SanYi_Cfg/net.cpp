#include "net.h"
#include "ui_net.h"

Net::Net(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Net)
{
    ui->setupUi(this);
}

Net::~Net()
{
    delete ui;
}

QJsonObject Net::GetCfgJson()
{
    QJsonObject obj;
    obj["addr"] = ui->le_addr->text();
    obj["port"] = ui->le_port->text().toInt();
    obj["slaveid"] = ui->le_slave->text().toInt();
    return obj;
}

void Net::SetCfgJson(const QJsonObject& obj)
{
    ui->le_addr->setText(obj["addr"].toString());
    ui->le_port->setText(QString("%1").arg(obj["port"].toInt()));
    ui->le_slave->setText(QString("%1").arg(obj["slaveid"].toInt()));
    return;
}
