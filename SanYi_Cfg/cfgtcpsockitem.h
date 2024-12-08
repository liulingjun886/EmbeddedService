#ifndef CFGTCPSOCKITEM_H
#define CFGTCPSOCKITEM_H

#include "tcpsockitem.h"
#include <QObject>

class CfgTcpSockItem : public TcpSockItem
{
    Q_OBJECT
public:
    CfgTcpSockItem();
    ~CfgTcpSockItem();

public:
    //数据接收完成
    virtual int CompleteRecv(const QByteArray &data);
    //连接断开函数调用
    virtual void onDisconnected();
    //连接建立后调用
    virtual void onConnected();
};

#endif // CFGTCPSOCKITEM_H
