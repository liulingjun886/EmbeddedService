#ifndef DEVICECFGDATA_H
#define DEVICECFGDATA_H

#include <QJsonObject>
#include "StatusInfo.h"

typedef struct stNetMsg
{
    int nType;
    int nLen;
    char data[0];
}NetMsg;

enum CFGTYEPE
{
    FILE_TRANSMIT_START = 1,		//文件开始上传
    FILE_TRANSMIT_COMMENT,			//文件类容传输
    FILE_TRANSMIT_FAILER,			//取消文件上传
    SYS_CONF_GET,					//系统配置获取
    SYS_CONF_SET,                   //系统配置设置
    DEV_CONF_GET,					//设备配置获取
    DEV_CONF_SET,					//设备配置设置
    NET_CFG,						//网络设置
};

class DeviceCfgData
{
public:
    int SendData(const QByteArray& data)
    {
        if(g_status_info.pSock)
            return g_status_info.pSock->SendData(data);
        return -1;
    }

    int SendData(const char *buff, int len)
    {
        if(g_status_info.pSock)
            return g_status_info.pSock->SendData(buff,len);
        return -1;
    }
public:
    virtual QJsonObject GetCfgJson()
    {
        QJsonObject obj;
        return obj;
    }
    virtual void SetCfgJson(const QJsonObject& obj)
    {
        (void)obj;
        return;
    }
    virtual int HandNetMsg(int nType, int nLen, const char* pData)
    {
        (void)(nType);
        (void)(nLen);
        (void)(pData);
        return 0;
    }

    virtual void GetSrvData()
    {
        return;
    }
};

#endif // DEVICECFGDATA_H
