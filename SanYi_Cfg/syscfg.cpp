#include "syscfg.h"
#include "ui_syscfg.h"

SysCfg::SysCfg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysCfg)
{
    ui->setupUi(this);
}

SysCfg::~SysCfg()
{
    delete ui;
}

int SysCfg::HandNetMsg(int nType, int nLen, const char* pData)
{
    if(SYS_CONF_GET == nType)
    {

    }
    else if(SYS_CONF_SET == nType)
    {

    }
    return 0;
}

void SysCfg::GetSrvData()
{
    QByteArray arr(sizeof(NetMsg),0);
    NetMsg* pMsg = (NetMsg*)arr.data();
    pMsg->nType = SYS_CONF_GET;
}

void SysCfg::on_pushButton_clicked()
{

}

