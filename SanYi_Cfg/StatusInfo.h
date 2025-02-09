#ifndef STATUSINFO_H
#define STATUSINFO_H
#include "tcpsockitem.h"
#include "dialog.h"

typedef struct stStatusInfo
{
    TcpSockItem* pSock;         //套接字对象
    Dialog* pMainDialog;        //主程序窗口
    QString strAppDir;          //程序路径
    int     subDeviceType;      //子设备管理类型
    stStatusInfo()
    {
        pSock = nullptr;
        pMainDialog = nullptr;
        subDeviceType = -1;
    }
}StatusInfo;


extern StatusInfo g_status_info;

#endif // STATUSINFO_H
