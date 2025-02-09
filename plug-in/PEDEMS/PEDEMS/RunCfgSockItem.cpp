#include "RunCfgSockItem.h"
#include <json/json.h>
#include <Log.h>
#include <vector>
#include "PedEms.h"

typedef struct stNetMsg
{
	int nType;
	int nLen;
	char data[0];
}NetMsg;

int RunCfgSockItem::onRecvCompelete(char* pData, UINT32 nLen)
{
	//校验长度
	if(nLen < 8)				//说明头部都没有收全
		return 0;

	int nType = *((int*)&pData[0]);
	int nLengh = *((int*)&pData[4]);

	if(nLengh + 8 > nLen)			//数据尚未接收完成
		return 0;

	std::string strValue="";
	strValue.append(&pData[8],nLengh);

	log_debug("ntype = %d, nLengh = %d", nType, nLen);

	//处理逻辑
	if(PedEms::GetInstance()->HandRunCfgMsg(this,nType,strValue))
		return -1;
	
	return nLengh + 8;
}

int RunCfgSockItem::SendData(int nType, char* pData, int nLen)
{
	std::vector<char> vecData;
	vecData.resize(nLen + 8);
	
	NetMsg* msg = (NetMsg*)vecData.data();
	msg->nType = nType;
	msg->nLen = nLen;
	if(nLen > 0)
		memcpy(&msg->data[0],pData,nLen);
	return CTcpSocketItem::SendData((char*)msg, nLen+8);
}