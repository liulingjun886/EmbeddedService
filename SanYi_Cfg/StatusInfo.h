#ifndef STATUSINFO_H
#define STATUSINFO_H
#include "tcpsockitem.h"
#include "dialog.h"

typedef struct stStatusInfo
{
    TcpSockItem* pSock;
    Dialog* pMainDialog;
    int nMainCode;
    int nSubCode;
}StatusInfo;


extern StatusInfo g_status_info;

#endif // STATUSINFO_H
