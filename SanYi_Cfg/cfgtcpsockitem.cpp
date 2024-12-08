#include "cfgtcpsockitem.h"
#include "StatusInfo.h"
#include <QMessageBox>
#include "DeviceCfgData.h"


CfgTcpSockItem::CfgTcpSockItem()
{

}

CfgTcpSockItem::~CfgTcpSockItem()
{

}

int CfgTcpSockItem::CompleteRecv(const QByteArray &data)
{
    if(data.length() < 8)
        return 0;
    NetMsg* pMsg = (NetMsg*)data.data();
    if(pMsg->nLen + 8 < data.length())
        return 0;
    qDebug()<< data;
    if(g_status_info.pMainDialog->HandNetMsg(pMsg->nType,pMsg->nLen,pMsg->data))
        return -1;
    return pMsg->nLen+8;
}

void CfgTcpSockItem::onDisconnected()
{
    TcpSockItem::onDisconnected();
    QMessageBox::warning(g_status_info.pMainDialog,"断开","设备连接已断开！");
}

void CfgTcpSockItem::onConnected()
{
    TcpSockItem::onConnected();
    QMessageBox::information(g_status_info.pMainDialog,"成功","设备连接成功！");
}
