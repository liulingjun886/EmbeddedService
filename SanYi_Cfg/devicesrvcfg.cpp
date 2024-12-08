#include "devicesrvcfg.h"
#include "ui_devicesrvcfg.h"
#include <QDebug>
#include "StatusInfo.h"
#include "cfgtcpsockitem.h"

DeviceSrvCfg::DeviceSrvCfg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceSrvCfg)
{
    ui->setupUi(this);
}

DeviceSrvCfg::~DeviceSrvCfg()
{
    delete ui;
}

void DeviceSrvCfg::on_pushButton_clicked()
{
    g_status_info.pSock = new CfgTcpSockItem;
    g_status_info.pSock->Init(ui->iPLineEdit->text(),(unsigned short)ui->PortLineEdit->text().toInt());
}

